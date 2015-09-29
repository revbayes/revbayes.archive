#include "DistributionNormal.h"
#include "PhyloBrownianProcessREML.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <cmath>


using namespace RevBayesCore;

PhyloBrownianProcessREML::PhyloBrownianProcessREML(const TypedDagNode<Tree> *t, const TypedDagNode<double> *homCR, const TypedDagNode<RbVector<double> > *hetCR, const TypedDagNode<double> *homSR, const TypedDagNode<RbVector<double> > *hetSR, size_t ns) :
    AbstractPhyloBrownianProcess( t, homCR, hetCR, homSR, hetSR, ns ),
    partialLikelihoods( std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) ) ),
    contrasts( std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->numNodes, std::vector<double>(this->numSites, 0) ) ) ),
    contrastUncertainty( std::vector<std::vector<double> >(2, std::vector<double>(this->numNodes, 0) ) ),
    activeLikelihood( std::vector<size_t>(this->numNodes, 0) ),
    changedNodes( std::vector<bool>(this->numNodes, false) ),
    dirtyNodes( std::vector<bool>(this->numNodes, true) )
{
    
    
    // We don'e want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    tau->incrementReferenceCount();
    
    // we need to reset the contrasts
    resetValue();
    
}


PhyloBrownianProcessREML::PhyloBrownianProcessREML(const PhyloBrownianProcessREML &n) :
    AbstractPhyloBrownianProcess( n ),
    partialLikelihoods( n.partialLikelihoods ),
    contrasts( n.contrasts ),
    contrastUncertainty( n.contrastUncertainty ),
    activeLikelihood( n.activeLikelihood ),
    changedNodes( n.changedNodes ),
    dirtyNodes( n.dirtyNodes )
{
    
    
    // We don't want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );
    tau->incrementReferenceCount();
    
}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloBrownianProcessREML::~PhyloBrownianProcessREML( void )
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



PhyloBrownianProcessREML* PhyloBrownianProcessREML::clone( void ) const
{
    
    return new PhyloBrownianProcessREML( *this );
}


double PhyloBrownianProcessREML::computeLnProbability( void )
{
    
    // we need to check here if we still are listining to this tree for change events
    // the tree could have been replaced without telling us
    if ( tau->getValue().getTreeChangeEventHandler().isListening( this ) == false )
    {
        tau->getValue().getTreeChangeEventHandler().addListener( this );
        dirtyNodes = std::vector<bool>(tau->getValue().getNumberOfNodes(), true);
    }
    
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



void PhyloBrownianProcessREML::fireTreeChangeEvent( const TopologyNode &n )
{
    
    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );
    
}


void PhyloBrownianProcessREML::keepSpecialization( DagNode* affecter )
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



void PhyloBrownianProcessREML::recursiveComputeLnProbability( const TopologyNode &node, size_t nodeIndex )
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
        const std::vector<double> &p_left  = this->partialLikelihoods[this->activeLikelihood[leftIndex]][leftIndex];
        const std::vector<double> &p_right = this->partialLikelihoods[this->activeLikelihood[rightIndex]][rightIndex];
        
        // get the per node and site contrasts
        std::vector<double> &mu_node  = this->contrasts[this->activeLikelihood[nodeIndex]][nodeIndex];
        const std::vector<double> &mu_left  = this->contrasts[this->activeLikelihood[leftIndex]][leftIndex];
        const std::vector<double> &mu_right = this->contrasts[this->activeLikelihood[rightIndex]][rightIndex];
        
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
            double lnl_node = RbStatistics::Normal::lnPdf(0, standDev, contrast);
            
            // sum up the probabilities of the contrasts
            p_node[i] = lnl_node + p_left[i] + p_right[i];
            
        } // end for-loop over all sites
        
    } // end if we need to compute something for this node.
    
}



void PhyloBrownianProcessREML::recursivelyFlagNodeDirty( const TopologyNode &n )
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


void PhyloBrownianProcessREML::resetValue( void )
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


void PhyloBrownianProcessREML::restoreSpecialization( DagNode* affecter )
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


double PhyloBrownianProcessREML::sumRootLikelihood( void )
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


void PhyloBrownianProcessREML::touchSpecialization( DagNode* affecter, bool touchAll )
{
    
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
void PhyloBrownianProcessREML::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == this->tau)
    {
        this->tau->getValue().getTreeChangeEventHandler().removeListener( this );
        this->tau->decrementReferenceCount();
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
        this->tau->getValue().getTreeChangeEventHandler().addListener( this );
        this->tau->incrementReferenceCount();
    }
    else
    {
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
    }
    
}

