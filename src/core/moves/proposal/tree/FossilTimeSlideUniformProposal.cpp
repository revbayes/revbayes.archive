#include "DistributionUniform.h"
#include "FossilTimeSlideUniformProposal.h"
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
FossilTimeSlideUniformProposal::FossilTimeSlideUniformProposal( StochasticNode<Tree> *n, TypedDagNode<double> *o ) : Proposal(),
    tree( n ),
    origin( o )
{
    // tell the base class to add the node
    addNode( tree );
    addNode( origin );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void FossilTimeSlideUniformProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
FossilTimeSlideUniformProposal* FossilTimeSlideUniformProposal::clone( void ) const
{
    
    return new FossilTimeSlideUniformProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& FossilTimeSlideUniformProposal::getProposalName( void ) const
{
    static std::string name = "FossilTimeSlideUniform";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Uniform-simplex proposal randomly changes some values of a simplex, although the other values
 * change too because of the renormalization.
 * First, some random indices are drawn. Then, the proposal draws a new somplex
 *   u ~ Uniform(val[index] * alpha)
 * where alpha is the tuning parameter.The new value is set to u.
 * The simplex is then renormalized.
 *
 * \return The hastings ratio.
 */
double FossilTimeSlideUniformProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree& tau = tree->getValue();
    
    size_t numFossils = 0;

    for (size_t i = 0; i < tau.getNumberOfNodes(); ++i)
    {
        TopologyNode* node = &tau.getNode(i);
        numFossils += node->isFossil();

    }

    if(numFossils == 0)
    {
        return 0;
    }

    // pick a random fossil node
    TopologyNode* node;
    do {
        double u = rng->uniform01();
        size_t index = size_t( std::floor(tau.getNumberOfNodes() * u) );
        node = &tau.getNode(index);
    } while ( !node->isFossil() );
    
    TopologyNode& parent = node->getParent();

    // we need to work with the times
    double parent_age  = parent.getAge();
    double my_age      = node->getAge();
    double sibling_Age = 0;

    if(node->isSampledAncestor())
    {
        TopologyNode *sibling = &parent.getChild( 0 );
        if ( sibling == node )
        {
            sibling = &parent.getChild( 1 );
        }

        sibling_Age = sibling->getAge();

        if(parent.isRoot())
        {
            if(origin == NULL)
                throw(RbException("Attempting to move root sampled ancestor, but no origin time provided."));

            parent_age = origin->getValue();
        }
        else
        {
            TopologyNode& grandParent = parent.getParent();

            parent_age = grandParent.getAge();
        }
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge = my_age;
    
    // draw new ages and compute the hastings ratio at the same time
    double my_new_age = sibling_Age + (parent_age - sibling_Age) * rng->uniform01();
    
    // set the age
    node->setAge( my_new_age );
    
    if(node->isSampledAncestor())
    {
        parent.setAge( my_new_age );
    }

    return 0.0;
    
}


/**
 *
 */
void FossilTimeSlideUniformProposal::prepareProposal( void )
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
void FossilTimeSlideUniformProposal::printParameterSummary(std::ostream &o) const
{
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void FossilTimeSlideUniformProposal::undoProposal( void )
{
    
    // undo the proposal
    storedNode->setAge( storedAge );
    
    if(storedNode->isSampledAncestor())
    {
        storedNode->getParent().setAge( storedAge );
    }

}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void FossilTimeSlideUniformProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if(oldN == tree)
    {
        tree = static_cast<StochasticNode<Tree>* >(newN) ;
    }
    else if(oldN == origin)
    {
        origin = static_cast<TypedDagNode<double>* >(newN) ;
    }
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void FossilTimeSlideUniformProposal::tune( double rate )
{
    
}

