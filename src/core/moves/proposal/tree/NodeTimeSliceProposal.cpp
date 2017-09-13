#include "DistributionUniform.h"
#include "NodeTimeSliceProposal.h"
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
NodeTimeSliceProposal::NodeTimeSliceProposal( StochasticNode<Tree> *n ) : Proposal(),
    variable( n )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void NodeTimeSliceProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
NodeTimeSliceProposal* NodeTimeSliceProposal::clone( void ) const
{
    
    return new NodeTimeSliceProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& NodeTimeSliceProposal::getProposalName( void ) const
{
    static std::string name = "NodeTimeSlideUniform";
    
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
double NodeTimeSliceProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree& tau = variable->getValue();
    
    if(tau.getNumberOfTips() <= 2)
    {
        return 0.0;
    }

    // pick a random node which is not the root and neithor the direct descendant of the root
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
    double child_age   = node->getChild( 0 ).getAge();
    if ( child_age < node->getChild( 1 ).getAge())
    {
        child_age = node->getChild( 1 ).getAge();
    }
    
    // now we store all necessary values
    storedNode = node;
    storedAge  = my_age;
    
    // get the current probability of the children and parents
    double current_ln_prob = variable->getLnProbability();
    
    const std::vector<DagNode*>& dag_children = variable->getChildren();
    for(size_t i = 0; i < dag_children.size(); ++i)
    {
        current_ln_prob += dag_children[i]->getLnProbability();
    }
    
    // sample a uniform random number between 0 and ln(current_ln_prob)
    // probably a smarter way to do this!
    double m = exp(current_ln_prob);
    double y = rng->uniform01() * m;
    double log_y = log(y);
    
    
    // initialize the interval
    double L = child_age;
    double R = parent_age;

    
    // perform the shrinkage procedure
    double new_age;
    double proposed_ln_probability;
    while (true)
    {
        
        // sample a uniform random number
        double u = rng->uniform01();
        
        // sample a new value for the node age
        new_age = L + u * (R - L);
        
        // set the value of the new node age
        
        // evaluate the probability given the new node age
        tau.getNode(node->getIndex()).setAge( new_age );
        variable->touch(true);
        
        proposed_ln_probability = variable->getLnProbability();
        for(size_t i = 0; i < dag_children.size(); ++i)
        {
            proposed_ln_probability += dag_children[i]->getLnProbability();
        }

        if ( log_y < proposed_ln_probability )
        {
            // we found a value within the acceptable interval
            break;
        }
        
        // otherwise, we have to shrink the interval
        if ( new_age < storedAge )
        {
            L = new_age;
        }
        else if ( new_age > storedAge )
        {
            R = new_age;
        }

    }
    
    
    
    double ln_hastings_ratio = current_ln_prob - proposed_ln_probability;
    
    return ln_hastings_ratio;
    
}


/**
 *
 */
void NodeTimeSliceProposal::prepareProposal( void )
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
void NodeTimeSliceProposal::printParameterSummary(std::ostream &o) const
{
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void NodeTimeSliceProposal::undoProposal( void )
{
    
    // undo the proposal
    storedNode->setAge( storedAge );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void NodeTimeSliceProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<Tree>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void NodeTimeSliceProposal::tune( double rate )
{
    
}

