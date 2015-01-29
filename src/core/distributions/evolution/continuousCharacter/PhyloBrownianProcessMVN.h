#ifndef PhyloBrownianProcessMVN_H
#define PhyloBrownianProcessMVN_H

#include "AbstractPhyloBrownianProcess.h"
#include "MatrixReal.h"

#include <vector>

namespace RevBayesCore {
    
    /**
     * @brief Homogeneous distribution of character state evolution along a tree class (PhyloCTMC).
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-01-23, version 1.0
     */
    template<class treeType>
    class PhyloBrownianProcessMVN : public AbstractPhyloBrownianProcess< treeType > {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        PhyloBrownianProcessMVN(const TypedDagNode<treeType> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, const TypedDagNode<double> *homRS, const TypedDagNode<RbVector<double> > *hetRS, size_t nSites );
        virtual                                                            ~PhyloBrownianProcessMVN(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual PhyloBrownianProcessMVN*                                    clone(void) const;                                                                      //!< Create an independent clone
        
        // non-virtual
        double                                                              computeLnProbability(void);
        
    protected:
        // virtual methods that may be overwritten, but then the derived class should call this methods
//        virtual void                                                        keepSpecialization(DagNode* affecter);
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        void                                                                resetValue( void );
//        virtual void                                                        restoreSpecialization(DagNode *restorer);
        double                                                              sumRootLikelihood(void);
        virtual void                                                        touchSpecialization(DagNode *toucher);
       
        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter
        
    private:
        double                                                              computeRootState(size_t siteIdx);
        std::set<size_t>                                                    recursiveComputeCovarianceMatrix( MatrixReal &m, const TopologyNode &node, size_t nodeIndex );
        
        const TypedDagNode< double >*                                       homogeneousRootState;
        const TypedDagNode< RbVector< double > >*                           heterogeneousRootState;
        
        size_t                                                              numTips;
        std::vector<std::vector<double> >                                   obs;
        MatrixReal                                                          phylogeneticCovarianceMatrix;
    };
    
}


#include "DistributionMultivariateNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <cmath>

template<class treeType>
RevBayesCore::PhyloBrownianProcessMVN<treeType>::PhyloBrownianProcessMVN(const TypedDagNode<treeType> *t,
                                                                         const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR,
                                                                         const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR,
                                                                         const TypedDagNode<double> *homRS, const TypedDagNode<RbVector<double> > *hetRS,
                                                                         size_t ns) :
    AbstractPhyloBrownianProcess< treeType >( t, homCR, hetCR, homSR, hetSR, ns ),
    homogeneousRootState( homRS ),
    heterogeneousRootState( hetRS ),
    numTips( t->getValue().getNumberOfTips() ),
    obs( std::vector<std::vector<double> >(this->numSites, std::vector<double>(numTips, 0.0) ) ),
    phylogeneticCovarianceMatrix( numTips, numTips )
{
    
    this->addParameter( homogeneousRootState );
    this->addParameter( heterogeneousRootState );
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class treeType>
RevBayesCore::PhyloBrownianProcessMVN<treeType>::~PhyloBrownianProcessMVN( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
}



template <class treeType>
RevBayesCore::PhyloBrownianProcessMVN<treeType>* RevBayesCore::PhyloBrownianProcessMVN<treeType>::clone( void ) const
{
    
    return new PhyloBrownianProcessMVN<treeType>( *this );
}


template<class treeType>
double RevBayesCore::PhyloBrownianProcessMVN<treeType>::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    phylogeneticCovarianceMatrix = MatrixReal(numTips, numTips);
    recursiveComputeCovarianceMatrix(phylogeneticCovarianceMatrix, root, rootIndex);
        
    // sum the partials up
    this->lnProb = sumRootLikelihood();
    
    return this->lnProb;
}


template<class treeType>
double RevBayesCore::PhyloBrownianProcessMVN<treeType>::computeRootState(size_t siteIdx)
{
    
    // second, get the clock rate for the branch
    double rootState;
    if ( this->heterogeneousRootState != NULL )
    {
        rootState = this->heterogeneousRootState->getValue()[siteIdx];
    }
    else
    {
        rootState = this->homogeneousRootState->getValue();
    }
    
    return rootState;
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessMVN<treeType>::resetValue( void )
{
    
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> siteIndices = std::vector<size_t>(this->numSites,0);
    size_t siteIndex = 0;
    for (size_t i = 0; i < this->numSites; ++i)
    {
        while ( this->value->isCharacterExcluded(siteIndex) )
        {
            siteIndex++;
            if ( siteIndex >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }
        siteIndices[i] = siteIndex;
        siteIndex++;
    }
    
    obs = std::vector<std::vector<double> >(this->numSites, std::vector<double>(numTips, 0.0) );
    
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();
    for (size_t site = 0; site < this->numSites; ++site)
    {
        
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                ContinuousTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                double &c = taxon.getCharacter(siteIndices[site]);
                obs[site][(*it)->getIndex()] = c;
            }
        }
    }
    
    
//    // finally we set all the flags for recomputation
//    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
//    {
//        (*it) = true;
//    }
//    
//    // flip the active likelihood pointers
//    for (size_t index = 0; index < changedNodes.size(); ++index)
//    {
//        activeLikelihood[index] = 0;
//        changedNodes[index] = true;
//    }
}


template<class treeType>
std::set<size_t> RevBayesCore::PhyloBrownianProcessMVN<treeType>::recursiveComputeCovarianceMatrix(MatrixReal &m, const TopologyNode &node, size_t nodeIndex)
{
    
    // I need to know all my children
    std::set<size_t> children;
    
    if ( node.isRoot() == false )
    {
        // get my scaled branch length
        double v = this->computeBranchTime(nodeIndex, node.getBranchLength() );
        
        if ( node.isTip() )
        {
            children.insert( nodeIndex );
            m[nodeIndex][nodeIndex] += v;
        }
        else
        {
            const TopologyNode &left = node.getChild(0);
            size_t leftIndex = left.getIndex();
            children = recursiveComputeCovarianceMatrix(m, left, leftIndex );
            
            const TopologyNode &right = node.getChild(1);
            size_t rightIndex = right.getIndex();
            std::set<size_t> childrenRight = recursiveComputeCovarianceMatrix(m, right, rightIndex );
            
            children.insert(childrenRight.begin(), childrenRight.end());
            
            // now we loop over all combination of the children pairs to add their variance terms
            for (std::set<size_t>::iterator i_itr = children.begin(); i_itr != children.end(); ++i_itr)
            {
                for (std::set<size_t>::iterator j_itr = children.begin(); j_itr != children.end(); ++j_itr)
                {
                    m[*i_itr][*j_itr] += v;
                }
            }
            
        }
    
    }
    else
    {
        
        for (size_t i = 0; i < node.getNumberOfChildren(); ++i)
        {
            const TopologyNode &child = node.getChild(i);
            size_t childIndex = child.getIndex();
            std::set<size_t> childrenRight = recursiveComputeCovarianceMatrix(m, child, childIndex );
        }
        
    }
    
    return children;
    
}



//template<class treeType>
//void RevBayesCore::PhyloBrownianProcessMVN<treeType>::restoreSpecialization( DagNode* affecter )
//{
//    
//    // reset the flags
//    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
//    {
//        (*it) = false;
//    }
//    
//    // restore the active likelihoods vector
//    for (size_t index = 0; index < changedNodes.size(); ++index)
//    {
//        // we have to restore, that means if we have changed the active likelihood vector
//        // then we need to revert this change
//        if ( changedNodes[index] == true )
//        {
//            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
//        }
//        
//        // set all flags to false
//        changedNodes[index] = false;
//    }
//    
//}


template<class treeType>
double RevBayesCore::PhyloBrownianProcessMVN<treeType>::sumRootLikelihood( void )
{
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    for (size_t site = 0; site < this->numSites; ++site)
    {
        std::vector<double> m = std::vector<double>(numTips, computeRootState(site) );
        
        double sigma = this->computeSiteRate(site);
        MatrixReal tmp = phylogeneticCovarianceMatrix * (sigma*sigma);
        MatrixReal cov = MatrixReal( tmp );
        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfCovariance(m, cov, obs[site]);
    }
    
    return sumPartialProbs;
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessMVN<treeType>::touchSpecialization( DagNode* affecter )
{
    
    bool touchAll = false;
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == homogeneousRootState )
    {
        
        
    }
    else if ( affecter == heterogeneousRootState )
    {
        
        
    }
    else if ( affecter == this->heterogeneousClockRates )
    {
        
        
    }
    else if ( affecter != this->tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    {
        touchAll = true;
    }
    
//    if ( touchAll )
//    {
//        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
//        {
//            (*it) = true;
//        }
//        
//        // flip the active likelihood pointers
//        for (size_t index = 0; index < changedNodes.size(); ++index)
//        {
//            if ( changedNodes[index] == false )
//            {
//                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
//                changedNodes[index] = true;
//            }
//        }
//    }
    
}


/** Swap a parameter of the distribution */
template<class treeType>
void RevBayesCore::PhyloBrownianProcessMVN<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == homogeneousRootState)
    {
        homogeneousRootState = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousRootState)
    {
        heterogeneousRootState = static_cast<const TypedDagNode< RbVector< double > >* >( newP );
    }
    else
    {
        AbstractPhyloBrownianProcess<treeType>::swapParameterInternal(oldP, newP);
    }
}



#endif
