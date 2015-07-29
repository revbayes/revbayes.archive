#include "SpeciesSubtreeScaleProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
SpeciesSubtreeScaleProposal::SpeciesSubtreeScaleProposal( StochasticNode<TimeTree> *sp, std::vector< StochasticNode<TimeTree> *> gt ) : Proposal(),
    speciesTree( sp ),
    geneTrees( gt )
{
    // tell the base class to add the node
    addNode( speciesTree );
    
    for (size_t i=0; i < geneTrees.size(); ++i)
    {
        addNode( geneTrees[i] );
    }
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void SpeciesSubtreeScaleProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
SpeciesSubtreeScaleProposal* SpeciesSubtreeScaleProposal::clone( void ) const
{
    
    return new SpeciesSubtreeScaleProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& SpeciesSubtreeScaleProposal::getProposalName( void ) const
{
    static std::string name = "SpeciesSubtreeScale";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * \return The hastings ratio.
 */
double SpeciesSubtreeScaleProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    TimeTree& tau = speciesTree->getValue();
    
    // pick a random node which is not the root and neither the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->isTip() );
    
    TopologyNode& parent = node->getParent();
    
    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // lower bound
    double min_age = 0.0;
    TreeUtilities::getOldestTip(&tau, node, min_age);
    
    // draw new ages and compute the hastings ratio at the same time
    double my_new_age = min_age + (parent_age - min_age) * rng->uniform01();
    
    my_new_age = my_age;
    
    double scaling_factor = my_new_age / my_age;
    
    size_t num_nodes = node->getNumberOfNodesInSubtree( false );
    
    for ( size_t i=0; i<geneTrees.size(); ++i )
    {
        // get the i-th gene tree
        TimeTree& gene_tree = geneTrees[i]->getValue();
        
        std::vector<TopologyNode*> nodes = getOldestNodesInPopulation(gene_tree, *node );
        
        for (size_t j=0; j<nodes.size(); ++j)
        {
            // add the number of nodes that we are going to scale in the subtree
            num_nodes += nodes[j]->getNumberOfNodesInSubtree( false );
            
            if ( nodes[j]->isTip() == true )
            {
                std::cerr << "Trying to scale a tip\n";
            }
            
            if ( nodes[j]->isRoot() == true )
            {
                std::cerr << "Trying to scale the root\n";
            }
            
            // rescale the subtree of this gene tree
            TreeUtilities::rescaleSubtree(&gene_tree, nodes[j], scaling_factor );
            
        }
        
    }
    
    // Sebastian: We need to work on a mechanism to make these proposal safe for non-ultrametric trees!
//    if (min_age != 0.0)
//    {
//        for (size_t i = 0; i < tau.getNumberOfTips(); i++)
//        {
//            if (tau.getNode(i).getAge() < 0.0)
//            {
//                return RbConstants::Double::neginf;
//            }
//        }
//    }
    
    
    // rescale the subtree of the species tree
    TreeUtilities::rescaleSubtree(&tau, node, scaling_factor );
    
    // compute the Hastings ratio
    double lnHastingsratio = (num_nodes > 1 ? log( scaling_factor ) * (num_nodes-1) : 0.0 );
    
    return lnHastingsratio;
}


std::vector<TopologyNode*> SpeciesSubtreeScaleProposal::getOldestNodesInPopulation( TimeTree &tau, TopologyNode &n )
{
    
    // I need all the oldest nodes/subtrees that have the same tips.
    // Those nodes need to be scaled too.
    
    // get the beginning and ending age of the population
    double max_age = -1.0;
    if ( n.isRoot() == false )
    {
        max_age = n.getParent().getAge();
    }
    
    // get all the taxa from the species tree that are descendants of node i
    std::vector<TopologyNode*> species_taxa;
    TreeUtilities::getTaxaInSubtree( &n, species_taxa );
    
    // get all the individuals
    std::set<TopologyNode*> individualTaxa;
    for (size_t i = 0; i < species_taxa.size(); ++i)
    {
        const std::string &name = species_taxa[i]->getName();
        std::vector<TopologyNode*> ind = tau.getTipNodesWithSpeciesName( name );
        for (size_t j = 0; j < ind.size(); ++j)
        {
            individualTaxa.insert( ind[j] );
        }
    }
    
    // create the set of the nodes within this population
    std::set<TopologyNode*> nodesInPopulationSet;
    
    // now go through all nodes in the gene
    while ( individualTaxa.empty() == false )
    {
        std::set<TopologyNode*>::iterator it = individualTaxa.begin();
        individualTaxa.erase( it );
        
        TopologyNode *geneNode = *it;
        
        // add this node to our list of node we need to scale, if:
        // a) this is the root node
        // b) this is not the root and the age of the parent node is larger than the parent's age of the species node
        if ( geneNode->isRoot() == false && ( max_age == -1.0 || max_age > geneNode->getParent().getAge() ) )
        {
            // push the parent to our current list
            individualTaxa.insert( &geneNode->getParent() );
        }
        else if ( geneNode->isTip() == false )
        {
            // add this node if it is within the age of our population
            nodesInPopulationSet.insert( geneNode );
        }
        
    }
    
    // convert the set into a vector
    std::vector<TopologyNode*> nodesInPopulation;
    for (std::set<TopologyNode*>::iterator it = nodesInPopulationSet.begin(); it != nodesInPopulationSet.end(); ++it)
    {
        nodesInPopulation.push_back( *it );
    }
    
    return nodesInPopulation;
}


/**
 *
 */
void SpeciesSubtreeScaleProposal::prepareProposal( void )
{
    
}


/**
 * Print the summary of the Proposal.
 *
 * The summary just contains the current value of the tuning parameter.
 * It is printed to the stream that it passed in.
 *
 * \param[in]     o     The stream to which we print the summary.
 */
void SpeciesSubtreeScaleProposal::printParameterSummary(std::ostream &o) const
{
    
    // no parameters
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void SpeciesSubtreeScaleProposal::undoProposal( void )
{
    // undo the proposal
    double sf = storedAge / storedNode->getAge();
    
    for ( size_t i=0; i<geneTrees.size(); ++i )
    {
        // get the i-th gene tree
        TimeTree& geneTree = geneTrees[i]->getValue();
        
        std::vector<TopologyNode*> nodes = getOldestNodesInPopulation(geneTree, *storedNode );
        
        for (size_t j=0; j<nodes.size(); ++j)
        {
            
            if ( nodes[j]->isTip() == true )
            {
                std::cerr << "Trying to scale a tip\n";
            }
            
            if ( nodes[j]->isRoot() == true )
            {
                std::cerr << "Trying to scale the root\n";
            }
            
            // rescale the subtree of this gene tree
            TreeUtilities::rescaleSubtree(&geneTree, nodes[j], sf );
            
        }
        
    }
    
    TreeUtilities::rescaleSubtree(&speciesTree->getValue(), storedNode, sf );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void SpeciesSubtreeScaleProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if ( oldN == speciesTree )
    {
        speciesTree = static_cast<StochasticNode<TimeTree>* >(newN) ;
    }
    else
    {
        size_t num_found_trees = 0;
        for ( size_t i=0; i<geneTrees.size(); ++i )
        {
            if ( oldN == geneTrees[i] )
            {
                geneTrees[i] = static_cast<StochasticNode<TimeTree>* >(newN) ;
                ++num_found_trees;
            }
        }
        
        if ( num_found_trees != 1 )
        {
            std::cerr << "Found " << num_found_trees << " DAG nodes matching to node \"" << oldN->getName() << "\".";
        }
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void SpeciesSubtreeScaleProposal::tune( double rate )
{
    
    // nothing to tune
    
}

