#ifndef PhyloBrownianProcessREML_H
#define PhyloBrownianProcessREML_H

#include "AbstractPhyloBrownianProcess.h"

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
    class PhyloBrownianProcessREML : public AbstractPhyloBrownianProcess< treeType >, public TreeChangeEventListener {
        
    public:
        // Note, we need the size of the alignment in the constructor to correctly simulate an initial state
        PhyloBrownianProcessREML(const TypedDagNode<treeType> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t nSites );
        PhyloBrownianProcessREML(const PhyloBrownianProcessREML &p );
        virtual                                                            ~PhyloBrownianProcessREML(void);                                                              //!< Virtual destructor
        
        // public member functions
        // pure virtual
        virtual PhyloBrownianProcessREML*                                   clone(void) const;                                                                      //!< Create an independent clone
        
        // non-virtual
        void                                                                fireTreeChangeEvent(const TopologyNode &n);                                             //!< The tree has changed and we want to know which part.
        double                                                              computeLnProbability(void);
        
    protected:
        // virtual methods that may be overwritten, but then the derived class should call this methods
        virtual void                                                        keepSpecialization(DagNode* affecter);
        void                                                                recursiveComputeLnProbability( const TopologyNode &node, size_t nodeIndex );
        void                                                                recursivelyFlagNodeDirty(const TopologyNode& n);
        void                                                                resetValue( void );
        virtual void                                                        restoreSpecialization(DagNode *restorer);
        double                                                              sumRootLikelihood(void);
        virtual void                                                        touchSpecialization(DagNode *toucher);

        // Parameter management functions.
        virtual void                                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);                         //!< Swap a parameter

        // the likelihoods
        std::vector<std::vector<std::vector<double> > >                     partialLikelihoods;
        std::vector<std::vector<std::vector<double> > >                     contrasts;
        std::vector<std::vector<double> >                                   contrastUncertainty;
        std::vector<size_t>                                                 activeLikelihood;
        
        // convenience variables available for derived classes too
        std::vector<bool>                                                   changedNodes;
        std::vector<bool>                                                   dirtyNodes;

    private:
                
    };
    
}


#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <cmath>

template<class treeType>
RevBayesCore::PhyloBrownianProcessREML<treeType>::PhyloBrownianProcessREML(const TypedDagNode<treeType> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t ns) :
    AbstractPhyloBrownianProcess< treeType >( t, homCR, hetCR, homSR, hetSR, ns ),
    partialLikelihoods( std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) ) ),
    contrasts( std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) ) ),
    contrastUncertainty( std::vector<std::vector<double> >(2, std::vector<double>(this->numNodes, 0) ) ),
    activeLikelihood( std::vector<size_t>(this->numNodes, 0) ),
    changedNodes( std::vector<bool>(this->numNodes, false) ),
    dirtyNodes( std::vector<bool>(this->numNodes, true) )
{
    
    
    // We don'e want tau to die before we die, or it can't remove us as listener
    this->tau->getValue().getTreeChangeEventHandler().addListener( this );
    this->tau->incrementReferenceCount();
    
}


template<class treeType>
RevBayesCore::PhyloBrownianProcessREML<treeType>::PhyloBrownianProcessREML(const PhyloBrownianProcessREML &n) :
    AbstractPhyloBrownianProcess<treeType>( n ),
    partialLikelihoods( n.partialLikelihoods ),
    contrasts( n.contrasts ),
    contrastUncertainty( n.contrastUncertainty ),
    activeLikelihood( n.activeLikelihood ),
    changedNodes( n.changedNodes ),
    dirtyNodes( n.dirtyNodes )
{
    
    
    // We don't want tau to die before we die, or it can't remove us as listener
    this->tau->getValue().getTreeChangeEventHandler().addListener( this );
    this->tau->incrementReferenceCount();
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
template<class treeType>
RevBayesCore::PhyloBrownianProcessREML<treeType>::~PhyloBrownianProcessREML( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( this->tau != NULL )
    {
        this->tau->getValue().getTreeChangeEventHandler().removeListener( this );
        if ( this->tau->decrementReferenceCount() == 0 )
        {
            delete this->tau;
        }
        
    }
    
}



template <class treeType>
RevBayesCore::PhyloBrownianProcessREML<treeType>* RevBayesCore::PhyloBrownianProcessREML<treeType>::clone( void ) const
{
    
    return new PhyloBrownianProcessREML<treeType>( *this );
}


template<class treeType>
double RevBayesCore::PhyloBrownianProcessREML<treeType>::computeLnProbability( void )
{
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    // only necessary if the root is actually dirty
    if ( this->dirtyNodes[rootIndex] )
    {
        
        // start by filling the likelihood vector for the children of the root
        if ( root.getNumberOfChildren() == 2 ) // rooted trees have two children for the root
        {
//            const TopologyNode &left = root.getChild(0);
//            size_t leftIndex = left.getIndex();
//            recursiveComputeLnProbability( left, leftIndex );
//            const TopologyNode &right = root.getChild(1);
//            size_t rightIndex = right.getIndex();
//            recursiveComputeLnProbability( right, rightIndex );
            
            recursiveComputeLnProbability( root, rootIndex );
//            computeRootLikelihood( rootIndex, leftIndex, rightIndex );
            
        }
        else if ( root.getNumberOfChildren() == 3 ) // unrooted trees have three children for the root
        {
            const TopologyNode &left = root.getChild(0);
            size_t leftIndex = left.getIndex();
            recursiveComputeLnProbability( left, leftIndex );
            const TopologyNode &right = root.getChild(1);
            size_t rightIndex = right.getIndex();
            recursiveComputeLnProbability( right, rightIndex );
            const TopologyNode &middle = root.getChild(2);
            size_t middleIndex = middle.getIndex();
            recursiveComputeLnProbability( middle, middleIndex );
            
//            computeRootLikelihood( rootIndex, leftIndex, rightIndex, middleIndex );
            
        }
        else
        {
            throw RbException("The root node has an unexpected number of children. Only 2 (for rooted trees) or 3 (for unrooted trees) are allowed.");
        }
        
        
        // sum the partials up
        this->lnProb = sumRootLikelihood();
        
    }
    return this->lnProb;
}



template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::fireTreeChangeEvent( const RevBayesCore::TopologyNode &n )
{
    
    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );
    
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::keepSpecialization( DagNode* affecter )
{
    
    // reset all flags
    for (std::vector<bool>::iterator it = this->dirtyNodes.begin(); it != this->dirtyNodes.end(); ++it)
    {
        (*it) = false;
    }
    
    for (std::vector<bool>::iterator it = this->changedNodes.begin(); it != this->changedNodes.end(); ++it)
    {
        (*it) = false;
    }
    
}



template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::recursiveComputeLnProbability( const RevBayesCore::TopologyNode &node, size_t nodeIndex )
{
    
    // check for recomputation
    if ( node.isTip() == false && dirtyNodes[nodeIndex] )
    {
        // mark as computed
        dirtyNodes[nodeIndex] = false;
        
        const TopologyNode &left = node.getChild(0);
        size_t leftIndex = left.getIndex();
        recursiveComputeLnProbability( left, leftIndex );
        
        const TopologyNode &right = node.getChild(1);
        size_t rightIndex = right.getIndex();
        recursiveComputeLnProbability( right, rightIndex );
        
        std::vector<double> &p_node  = this->partialLikelihoods[this->activeLikelihood[nodeIndex]][nodeIndex];
        std::vector<double> &p_left  = this->partialLikelihoods[this->activeLikelihood[leftIndex]][leftIndex];
        std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihood[rightIndex]][rightIndex];

        // get the per node and site contrasts
        std::vector<double> &mu_node  = this->contrasts[this->activeLikelihood[nodeIndex]][nodeIndex];
        std::vector<double> &mu_left  = this->contrasts[this->activeLikelihood[leftIndex]][leftIndex];
        std::vector<double> &mu_right = this->contrasts[this->activeLikelihood[rightIndex]][rightIndex];

        // get the propagated uncertainties
        double delta_left  = this->contrastUncertainty[this->activeLikelihood[leftIndex]][leftIndex];
        double delta_right = this->contrastUncertainty[this->activeLikelihood[rightIndex]][rightIndex];

        // get the scaled branch lengths
        double v_left  = this->computeBranchTime(leftIndex, left.getBranchLength());
        double v_right = this->computeBranchTime(rightIndex, right.getBranchLength());
        
        // add the propagated uncertainty to the branch lengths
        double t_left  = v_left  + delta_left;
        double t_right = v_right + delta_right;
        
        // set delta_node = (t_l*t_r)/(t_l+t_r);
        this->contrastUncertainty[this->activeLikelihood[nodeIndex]][nodeIndex] = (t_left*t_right) / (t_left+t_right);
        
        double stdev = sqrt(t_left+t_right);
        for (int i=0; i<this->numSites; i++)
        {
            
            mu_node[i] = (mu_left[i]*t_right + mu_right[i]*t_left) / (t_left+t_right);
            
            // get the site specific rate of evolution
            double standDev = this->computeSiteRate(i) * stdev;
            
            // compute the contrasts for this site and node
            double contrast = mu_left[i] - mu_right[i];
            
            // compute the probability for the contrasts at this node
            double lnl_node = RbStatistics::Normal::lnPdf(contrast, standDev, 0);
            
            // sum up the probabilities of the contrasts
            p_node[i] = lnl_node + p_left[i] + p_right[i];
            
        } // end for-loop over all sites
        
    } // end if we need to compute something for this node.
    
}



template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::recursivelyFlagNodeDirty( const RevBayesCore::TopologyNode &n )
{
    
    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();
    
    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( !dirtyNodes[index] )
    {
        // the root doesn't have an ancestor
        if ( !n.isRoot() )
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        
        // set the flag
        dirtyNodes[index] = true;
        
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( changedNodes[index] == false )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
            changedNodes[index] = true;
        }
        
    }
    
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::resetValue( void )
{
    
    // check if the vectors need to be resized
    partialLikelihoods = std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) );
    contrasts = std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) );
    contrastUncertainty = std::vector<std::vector<double> >(2, std::vector<double>(this->numNodes, 0) );
    
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
    
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();
    for (size_t site = 0; site < this->numSites; ++site)
    {
        
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                ContinuousTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                double &c = taxon.getCharacter(siteIndices[site]);
                contrasts[0][(*it)->getIndex()][site] = c;
                contrasts[1][(*it)->getIndex()][site] = c;
                contrastUncertainty[0][(*it)->getIndex()] = 0;
                contrastUncertainty[1][(*it)->getIndex()] = 0;
            }
        }
    }
    
    
    // finally we set all the flags for recomputation
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
    {
        (*it) = true;
    }
    
    // flip the active likelihood pointers
    for (size_t index = 0; index < changedNodes.size(); ++index)
    {
        activeLikelihood[index] = 0;
        changedNodes[index] = true;
    }
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::restoreSpecialization( DagNode* affecter )
{
    
    // reset the flags
    for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
    {
        (*it) = false;
    }
    
    // restore the active likelihoods vector
    for (size_t index = 0; index < changedNodes.size(); ++index)
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changedNodes[index] == true )
        {
            activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
        }
        
        // set all flags to false
        changedNodes[index] = false;
    }
    
}


template<class treeType>
double RevBayesCore::PhyloBrownianProcessREML<treeType>::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    // get the pointers to the partial likelihoods of the left and right subtree
    std::vector<double> &p_node  = this->partialLikelihoods[this->activeLikelihood[nodeIndex]][nodeIndex];
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    for (size_t site = 0; site < this->numSites; ++site)
    {
        sumPartialProbs += p_node[site];
    }
    
    return sumPartialProbs;
}


template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::touchSpecialization( DagNode* affecter )
{
    
    bool touchAll = false;
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == this->heterogeneousClockRates )
    {
        
        const std::set<size_t> &indices = this->heterogeneousClockRates->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 )
        {
            // just flag everyting for recomputation
            touchAll = true;
        }
        else
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it)
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            }
        }
    }
    else if ( affecter != this->tau ) // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    {
        touchAll = true;
    }
    
    if ( touchAll )
    {
        for (std::vector<bool>::iterator it = dirtyNodes.begin(); it != dirtyNodes.end(); ++it)
        {
            (*it) = true;
        }
        
        // flip the active likelihood pointers
        for (size_t index = 0; index < changedNodes.size(); ++index)
        {
            if ( changedNodes[index] == false )
            {
                activeLikelihood[index] = (activeLikelihood[index] == 0 ? 1 : 0);
                changedNodes[index] = true;
            }
        }
    }
    
}


/** Swap a parameter of the distribution */
template<class treeType>
void RevBayesCore::PhyloBrownianProcessREML<treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == this->tau)
    {
        this->tau->getValue().getTreeChangeEventHandler().removeListener( this );
        this->tau->decrementReferenceCount();
        AbstractPhyloBrownianProcess<treeType>::swapParameterInternal(oldP, newP);
        this->tau->getValue().getTreeChangeEventHandler().addListener( this );
        this->tau->incrementReferenceCount();
    }
    else
    {
        AbstractPhyloBrownianProcess<treeType>::swapParameterInternal(oldP, newP);
    }
    
}



#endif
