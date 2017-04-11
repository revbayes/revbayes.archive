#include "DistributionMultivariateNormal.h"
#include "PhyloMultivariateBrownianProcessREML.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TopologyNode.h"

#include <cmath>


using namespace RevBayesCore;

PhyloMultivariateBrownianProcessREML::PhyloMultivariateBrownianProcessREML(const TypedDagNode<Tree> *t, const TypedDagNode<MatrixReal> *c, size_t ns) :
    AbstractPhyloBrownianProcess( t, ns ),
    rate_matrix( c ),
    partial_likelihoods( std::vector<std::vector<double> >(2, std::vector<double>(this->num_nodes, 0) ) ),
    contrasts( std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->num_nodes, std::vector<double>(this->num_sites, 0.0) ) ) ),
    contrast_uncertainty( std::vector<std::vector<double> >(2, std::vector<double>(this->num_nodes, 0) ) ),
    active_likelihood( std::vector<size_t>(this->num_nodes, 0) ),
    changed_nodes( std::vector<bool>(this->num_nodes, false) ),
    dirty_nodes( std::vector<bool>(this->num_nodes, true) )
{
    
    // add the parameters to our set
    this->addParameter( rate_matrix );
    
    // make sure the rate matrix is inverted using Cholesky decomposition
    rate_matrix->getValue().setCholesky(true);
    
    // compute the inverse variance-covariance matrix (the precision matrix)
    precision_matrix = rate_matrix->getValue().computeInverse();

    // We don't want tau to die before we die, or it can't remove us as listener
    tau->getValue().getTreeChangeEventHandler().addListener( this );

    // now we need to reset the valuz
    this->redrawValue();
    
    // we need to reset the contrasts
    resetValue();

}


/**
 * Destructor. Because we added ourselves as a reference to tau when we added a listener to its
 * TreeChangeEventHandler, we need to remove ourselves as a reference and possibly delete tau
 * when we die. All other parameters are handled by others.
 */
PhyloMultivariateBrownianProcessREML::~PhyloMultivariateBrownianProcessREML( void )
{
    // We don't delete the params, because they might be used somewhere else too. The model needs to do that!
    
    // remove myself from the tree listeners
    if ( tau != NULL )
    {
        tau->getValue().getTreeChangeEventHandler().removeListener( this );
    }
    
}



PhyloMultivariateBrownianProcessREML* PhyloMultivariateBrownianProcessREML::clone( void ) const
{
    
    return new PhyloMultivariateBrownianProcessREML( *this );
}


double PhyloMultivariateBrownianProcessREML::computeLnProbability( void )
{
    
    // we need to check here if we still are listining to this tree for change events
    // the tree could have been replaced without telling us
    if ( tau->getValue().getTreeChangeEventHandler().isListening( this ) == false )
    {
        tau->getValue().getTreeChangeEventHandler().addListener( this );
        dirty_nodes = std::vector<bool>(tau->getValue().getNumberOfNodes(), true);
    }
    
    precision_matrix = rate_matrix->getValue().computeInverse();
    
    // compute the ln probability by recursively calling the probability calculation for each node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // we start with the root and then traverse down the tree
    size_t rootIndex = root.getIndex();
    
    // only necessary if the root is actually dirty
    if ( this->dirty_nodes[rootIndex] )
    {
        
        recursiveComputeLnProbability( root, rootIndex );
        
        // return the likelihood at the root
        this->lnProb = this->partial_likelihoods[this->active_likelihood[rootIndex]][rootIndex];
        
    }
    
    return this->lnProb;
    
}



void PhyloMultivariateBrownianProcessREML::fireTreeChangeEvent( const TopologyNode &n, const unsigned& m )
{
    
    // call a recursive flagging of all node above (closer to the root) and including this node
    recursivelyFlagNodeDirty( n );
    
}


void PhyloMultivariateBrownianProcessREML::keepSpecialization( DagNode* affecter )
{
    
    // reset all flags
    for (std::vector<bool>::iterator it = this->dirty_nodes.begin(); it != this->dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }
    
    for (std::vector<bool>::iterator it = this->changed_nodes.begin(); it != this->changed_nodes.end(); ++it)
    {
        (*it) = false;
    }
    
}


void PhyloMultivariateBrownianProcessREML::recursiveComputeLnProbability( const TopologyNode &node, size_t node_index )
{

    // check for recomputation
    if ( node.isTip() == false && dirty_nodes[node_index] )
    {
        // mark as computed
        dirty_nodes[node_index] = false;

        double              &p_node  = this->partial_likelihoods[this->active_likelihood[node_index]][node_index];
        std::vector<double> &mu_node = this->contrasts[this->active_likelihood[node_index]][node_index];

        // get the number of children
        size_t num_children = node.getNumberOfChildren();
        
        for (size_t j = 1; j < num_children; ++j)
        {
        
            size_t left_index = node_index;
            const TopologyNode *left = &node;
            if ( j == 1 )
            {
                left = &node.getChild(0);
                left_index = left->getIndex();
                recursiveComputeLnProbability( *left, left_index );
            }
            
            const TopologyNode &right = node.getChild(j);
            size_t right_index = right.getIndex();
            recursiveComputeLnProbability( right, right_index );

            const double &p_left  = this->partial_likelihoods[this->active_likelihood[left_index]][left_index];
            const double &p_right = this->partial_likelihoods[this->active_likelihood[right_index]][right_index];

            // get the per node and site contrasts
            const std::vector<double> &mu_left  = this->contrasts[this->active_likelihood[left_index]][left_index];
            const std::vector<double> &mu_right = this->contrasts[this->active_likelihood[right_index]][right_index];

            // get the propagated uncertainties
            double delta_left  = this->contrast_uncertainty[this->active_likelihood[left_index]][left_index];
            double delta_right = this->contrast_uncertainty[this->active_likelihood[right_index]][right_index];

            // get the scaled branch lengths
            double v_left  = 0;
            if ( j == 1 )
            {
                v_left = this->computeBranchTime(left_index, left->getBranchLength());
            }
            double v_right = this->computeBranchTime(right_index, right.getBranchLength());

            // add the propagated uncertainty to the branch lengths
            double t_left  = v_left  + delta_left;
            double t_right = v_right + delta_right;

            // set delta_node = (t_l*t_r)/(t_l+t_r);
            this->contrast_uncertainty[this->active_likelihood[node_index]][node_index] = (t_left * t_right) / (t_left + t_right);

            double branch_length = t_left + t_right;
            
            std::vector<double> contrasts(num_sites);
            std::vector<double> means(num_sites, 0.0);
            for (size_t i = 0; i < this->num_sites; ++i)
            {
                contrasts[i] = mu_left[i] - mu_right[i];
                mu_node[i] = (mu_left[i] * t_right + mu_right[i] * t_left) / (t_left + t_right);
            }
            
//            double lnl_contrast = RbStatistics::MultivariateNormal::lnPdfCovariance(means, rate_matrix->getValue(), contrasts, branch_length);
            double lnl_contrast = RbStatistics::MultivariateNormal::lnPdfPrecision(means, precision_matrix, contrasts, branch_length);
            p_node = lnl_contrast + p_left + p_right;
            
        } // end for-loop over all children
        
    } // end if we need to compute something for this node.

}

void PhyloMultivariateBrownianProcessREML::recursivelyFlagNodeDirty( const TopologyNode &n )
{
    
    // we need to flag this node and all ancestral nodes for recomputation
    size_t index = n.getIndex();
    
    // if this node is already dirty, the also all the ancestral nodes must have been flagged as dirty
    if ( !dirty_nodes[index] )
    {
        // the root doesn't have an ancestor
        if ( !n.isRoot() )
        {
            recursivelyFlagNodeDirty( n.getParent() );
        }
        
        // set the flag
        dirty_nodes[index] = true;
        
        // if we previously haven't touched this node, then we need to change the active likelihood pointer
        if ( changed_nodes[index] == false )
        {
            active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
            changed_nodes[index] = true;
        }
        
    }
    
}


void PhyloMultivariateBrownianProcessREML::resetValue( void )
{
    
    // check if the vectors need to be resized
    partial_likelihoods = std::vector<std::vector<double> >(2, std::vector<double>(this->num_nodes, 0) );
    contrasts = std::vector<std::vector<std::vector<double> > >(2, std::vector<std::vector<double> >(this->num_nodes, std::vector<double>(this->num_sites, 0) ) );
    contrast_uncertainty = std::vector<std::vector<double> >(2, std::vector<double>(this->num_nodes, 0) );
    
    // create a vector with the correct site indices
    // some of the sites may have been excluded
    std::vector<size_t> site_indices = std::vector<size_t>(this->num_sites,0);
    size_t site_index = 0;
    for (size_t i = 0; i < this->num_sites; ++i)
    {
        while ( this->value->isCharacterExcluded(site_index) )
        {
            ++site_index;
            if ( site_index >= this->value->getNumberOfCharacters()  )
            {
                throw RbException( "The character matrix cannot set to this variable because it does not have enough included characters." );
            }
        }
        site_indices[i] = site_index;
        ++site_index;
    }
    
    std::vector<TopologyNode*> nodes = this->tau->getValue().getNodes();
    for (size_t site = 0; site < this->num_sites; ++site)
    {
        
        for (std::vector<TopologyNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
        {
            if ( (*it)->isTip() )
            {
                ContinuousTaxonData& taxon = this->value->getTaxonData( (*it)->getName() );
                double &c = taxon.getCharacter(site_indices[site]);
                contrasts[0][(*it)->getIndex()][site] = c;
                contrasts[1][(*it)->getIndex()][site] = c;
                contrast_uncertainty[0][(*it)->getIndex()] = 0;
                contrast_uncertainty[1][(*it)->getIndex()] = 0;
            }
        }
    }
    
    
    // finally we set all the flags for recomputation
    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    {
        (*it) = true;
    }
    
    // flip the active likelihood pointers
    for (size_t index = 0; index < changed_nodes.size(); ++index)
    {
        active_likelihood[index] = 0;
        changed_nodes[index] = true;
    }
    
}


void PhyloMultivariateBrownianProcessREML::restoreSpecialization( DagNode* affecter )
{
    
    
//    if ( affecter == rate_matrix )
//    {
//        // compute the inverse variance-covariance matrix (the precision matrix)
//        precision_matrix = rate_matrix->getValue().computeInverse();
//    }
    
    // reset the flags
    for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
    {
        (*it) = false;
    }
    
    // restore the active likelihoods vector
    for (size_t index = 0; index < changed_nodes.size(); ++index)
    {
        // we have to restore, that means if we have changed the active likelihood vector
        // then we need to revert this change
        if ( changed_nodes[index] == true )
        {
            active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
        }
        
        // set all flags to false
        changed_nodes[index] = false;
    }
    
}


void PhyloMultivariateBrownianProcessREML::simulateRecursively( const TopologyNode &node, std::vector< ContinuousTaxonData > &taxa)
{
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t node_index = node.getIndex();
    const ContinuousTaxonData &parent = taxa[ node_index ];
    
    std::vector<double> parent_state(num_sites, 0.0);
    for(size_t i = 0; i < num_sites; ++i)
    {
        parent_state[i] = parent.getCharacter(i);
    }
    
    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);
        
        // get the branch length for this child
        double branch_length = child.getBranchLength();
        
        ContinuousTaxonData &taxon = taxa[ child.getIndex() ];
        std::vector<double> c = RbStatistics::MultivariateNormal::rvPrecision(parent_state, precision_matrix, *rng, branch_length);
        
        for ( size_t i = 0; i < num_sites; ++i )
        {
            // add the character to the sequence
            taxon.addCharacter( c[i] );
        }
        
        if ( child.isTip() )
        {
            taxon.setTaxon( child.getTaxon() );
        }
        else
        {
            // recursively simulate the sequences
            simulateRecursively( child, taxa );
        }
        
    }
    
}


std::vector<double> PhyloMultivariateBrownianProcessREML::simulateRootCharacters(size_t n)
{
    
    std::vector<double> chars = std::vector<double>(num_sites, 0);
    for (size_t i=0; i<num_sites; ++i)
    {
        chars[i] = 0.0;
    }
    
    return chars;
}


void PhyloMultivariateBrownianProcessREML::touchSpecialization( DagNode* affecter, bool touchAll )
{
 
//    if ( affecter == rate_matrix )
//    {
//        // compute the inverse variance-covariance matrix (the precision matrix)
//        precision_matrix = rate_matrix->getValue().computeInverse();
//    }
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == this->heterogeneous_clock_rates )
    {
        
        const std::set<size_t> &indices = this->heterogeneous_clock_rates->getTouchedElementIndices();
        
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
        for (std::vector<bool>::iterator it = dirty_nodes.begin(); it != dirty_nodes.end(); ++it)
        {
            (*it) = true;
        }
        
        // flip the active likelihood pointers
        for (size_t index = 0; index < changed_nodes.size(); ++index)
        {
            if ( changed_nodes[index] == false )
            {
                active_likelihood[index] = (active_likelihood[index] == 0 ? 1 : 0);
                changed_nodes[index] = true;
            }
        }
        
    }
    
}


/** Swap a parameter of the distribution */
void PhyloMultivariateBrownianProcessREML::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == this->tau)
    {
        this->tau->getValue().getTreeChangeEventHandler().removeListener( this );
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
        this->tau->getValue().getTreeChangeEventHandler().addListener( this );
    }
    if (oldP == this->rate_matrix)
    {
        rate_matrix = static_cast<const TypedDagNode< MatrixReal >* >( newP );
    }
    else
    {
        AbstractPhyloBrownianProcess::swapParameterInternal(oldP, newP);
    }
    
}

