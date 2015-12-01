#include "PhyloBrownianProcessMVN.h"
#include "DistributionMultivariateNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <cmath>


using namespace RevBayesCore;

PhyloBrownianProcessMVN::PhyloBrownianProcessMVN(const TypedDagNode<Tree> *t,
                                                 const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR,
                                                 const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR,
                                                 const TypedDagNode<double> *homRS, const TypedDagNode<RbVector<double> > *hetRS,
                                                 size_t ns) :
    AbstractPhyloBrownianProcess( t, homCR, hetCR, homSR, hetSR, ns ),
    homogeneousRootState( homRS ),
    heterogeneousRootState( hetRS ),
    numTips( t->getValue().getNumberOfTips() ),
    obs( std::vector<std::vector<double> >(this->numSites, std::vector<double>(numTips, 0.0) ) ),
    phylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    storedPhylogeneticCovarianceMatrix( new MatrixReal(numTips, numTips) ),
    inversePhylogeneticCovarianceMatrix( numTips, numTips ),
    changedCovariance(false),
    needsCovarianceRecomputation( true ),
    needsScaleRecomputation( true )
{
    
    addParameter( homogeneousRootState );
    addParameter( heterogeneousRootState );
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloBrownianProcessMVN::~PhyloBrownianProcessMVN( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
}



PhyloBrownianProcessMVN* RevBayesCore::PhyloBrownianProcessMVN::clone( void ) const
{
    
    return new PhyloBrownianProcessMVN( *this );
}


double PhyloBrownianProcessMVN::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    if ( needsCovarianceRecomputation == true )
    {
        // perhaps there is a more efficient way to reset the matrix to 0.
        phylogeneticCovarianceMatrix = new MatrixReal(numTips, numTips);
        recursiveComputeCovarianceMatrix(*phylogeneticCovarianceMatrix, root, rootIndex);
        needsCovarianceRecomputation = false;
        inversePhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix->computeInverse();
    }
    
    // sum the partials up
    this->lnProb = sumRootLikelihood();
    
    return this->lnProb;
}


double PhyloBrownianProcessMVN::computeRootState(size_t siteIdx)
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




void PhyloBrownianProcessMVN::keepSpecialization( DagNode* affecter )
{
    
    // reset the flags
    changedCovariance = false;
    needsCovarianceRecomputation = false;
    
}


void PhyloBrownianProcessMVN::resetValue( void )
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
    
    
    // finally we set all the flags for recomputation
    needsCovarianceRecomputation = true;
    needsScaleRecomputation = true;
    
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


std::set<size_t> RevBayesCore::PhyloBrownianProcessMVN::recursiveComputeCovarianceMatrix(MatrixReal &m, const TopologyNode &node, size_t nodeIndex)
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



void RevBayesCore::PhyloBrownianProcessMVN::restoreSpecialization( DagNode* affecter )
{
    
    // reset the flags
    if ( changedCovariance == true )
    {
        changedCovariance = false;
        needsCovarianceRecomputation = false;
        
        MatrixReal *tmp = phylogeneticCovarianceMatrix;
        phylogeneticCovarianceMatrix = storedPhylogeneticCovarianceMatrix;
        storedPhylogeneticCovarianceMatrix = tmp;
        
    }
    
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
}


double RevBayesCore::PhyloBrownianProcessMVN::sumRootLikelihood( void )
{
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    for (size_t site = 0; site < this->numSites; ++site)
    {
        std::vector<double> m = std::vector<double>(numTips, computeRootState(site) );
        
        double sigma = this->computeSiteRate(site);
        //        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfCovariance(m, *phylogeneticCovarianceMatrix, obs[site], sigma*sigma);
        sumPartialProbs += RbStatistics::MultivariateNormal::lnPdfPrecision(m, inversePhylogeneticCovarianceMatrix, obs[site], sigma*sigma);
    }
    
    return sumPartialProbs;
}


void RevBayesCore::PhyloBrownianProcessMVN::touchSpecialization( DagNode* affecter, bool touchAll )
{
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == homogeneousRootState )
    {
        
        
    }
    else if ( affecter == heterogeneousRootState )
    {
        
        
    }
    else if ( affecter == this->homogeneousClockRate )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            MatrixReal *tmp = phylogeneticCovarianceMatrix;
            phylogeneticCovarianceMatrix = storedPhylogeneticCovarianceMatrix;
            storedPhylogeneticCovarianceMatrix = tmp;
        }
        changedCovariance = true;
        
    }
    else if ( affecter == this->heterogeneousClockRates )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            storedPhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix;
        }
        changedCovariance = true;
        
    }
    else if ( affecter == this->tau )
    {
        needsCovarianceRecomputation = true;
        if ( changedCovariance == false )
        {
            storedPhylogeneticCovarianceMatrix = phylogeneticCovarianceMatrix;
        }
        changedCovariance = true;
        
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
void RevBayesCore::PhyloBrownianProcessMVN::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
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
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
    }
}


