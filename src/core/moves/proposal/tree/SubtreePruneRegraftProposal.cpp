#include "SubtreePruneRegraftProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "Topology.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
SubtreePruneRegraftProposal::SubtreePruneRegraftProposal( StochasticNode<Topology> *n ) : Proposal(),
    tree( n )
{
    // tell the base class to add the node
    addNode( tree );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void SubtreePruneRegraftProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
SubtreePruneRegraftProposal* SubtreePruneRegraftProposal::clone( void ) const
{
    
    return new SubtreePruneRegraftProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& SubtreePruneRegraftProposal::getProposalName( void ) const
{
    static std::string name = "SubtreePruneRegraft";
    
    return name;
}


bool SubtreePruneRegraftProposal::isDescendant(const TopologyNode &n, const TopologyNode &p) {
    
    if ( n.isRoot() ) {
        return false;
    }
    
    if ( &n == &p ) {
        return true;
    }
    
    return isDescendant(n.getParent(), p);
}


/**
 * Perform the proposal.
 *
 * A SPR proposal.
 *
 * \return The hastings ratio.
 */
double SubtreePruneRegraftProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Topology& tau = tree->getValue();
    
    // pick a random node which is not the root and neithor the direct descendant of the root
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( node->isRoot() || node->getParent().isRoot() );
    
    // now we store all necessary values
    storedChoosenNode   = node;
    TopologyNode &parent = node->getParent();
    TopologyNode &grandparent = parent.getParent();
    storedBrother = &parent.getChild( 0 );
    
    // check if we got the correct child
    if ( node == storedBrother ) {
        storedBrother = &parent.getChild( 1 );
    }
    
    // pick a random new parent node
    TopologyNode* newBrother;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        newBrother = &tau.getNode(index);
    } while ( newBrother->isRoot() || isDescendant(*newBrother,parent));
    
    TopologyNode &newGrandparent = newBrother->getParent();
    
    // now prune
    grandparent.removeChild( &parent );
    parent.removeChild( storedBrother );
    grandparent.addChild( storedBrother );
    storedBrother->setParent( &grandparent );
    
    // re-attach
    newGrandparent.removeChild( newBrother );
    parent.addChild( newBrother );
    newGrandparent.addChild( &parent );
    parent.setParent( &newGrandparent );
    newBrother->setParent( &parent );
    
    return 0.0;
    
}


/**
 *
 */
void SubtreePruneRegraftProposal::prepareProposal( void )
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
void SubtreePruneRegraftProposal::printParameterSummary(std::ostream &o) const
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
void SubtreePruneRegraftProposal::undoProposal( void )
{
    
    // undo the proposal
    TopologyNode &parent = storedChoosenNode->getParent();
    TopologyNode &grandparent = parent.getParent();
    TopologyNode* oldBrother = &parent.getChild( 0 );
    TopologyNode &newGrandparent = storedBrother->getParent();
    
    // check if we got the correct child
    if ( storedChoosenNode == oldBrother )
    {
        oldBrother = &parent.getChild( 1 );
    }
    
    // now prune
    grandparent.removeChild( &parent );
    parent.removeChild( oldBrother );
    grandparent.addChild( oldBrother );
    oldBrother->setParent( &grandparent );
    
    // re-attach
    newGrandparent.removeChild( storedBrother );
    parent.addChild( storedBrother );
    newGrandparent.addChild( &parent );
    parent.setParent( &newGrandparent );
    storedBrother->setParent( &parent );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void SubtreePruneRegraftProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    tree = static_cast<StochasticNode<Topology>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void SubtreePruneRegraftProposal::tune( double rate )
{
    
    // nothing to tune
    
}

