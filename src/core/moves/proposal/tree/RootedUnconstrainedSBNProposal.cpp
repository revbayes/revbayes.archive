#include "RootedUnconstrainedSBNProposal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
RootedUnconstrainedSBNProposal::RootedUnconstrainedSBNProposal( StochasticNode<Tree> *n, std::vector< StochasticNode<double> *> b, const SBNParameters sbn ) : Proposal(),
    tree( n ),
    variables( b ),
    length( variables.size() ),
    // SBNParameters( sbn ),
    SBNDistribution(UnconstrainedSBN(sbn,true))
{
    // tell the base class to add the node
    addNode( tree );

    // tell the base class to add the node
    // also ensure stored_branch_lengths is correct size
    for (std::vector< StochasticNode<double> *>::const_iterator it = variables.begin(); it != variables.end(); it++)
    {
        addNode( *it );
        stored_branch_lengths.push_back(-10.0);
    }

}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void RootedUnconstrainedSBNProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
RootedUnconstrainedSBNProposal* RootedUnconstrainedSBNProposal::clone( void ) const
{

    return new RootedUnconstrainedSBNProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& RootedUnconstrainedSBNProposal::getProposalName( void ) const
{
    static std::string name = "UnconstrainedRootedSBN";

    return name;
}


double RootedUnconstrainedSBNProposal::getProposalTuningParameter( void ) const
{
    // this proposal has no tuning parameter
    return RbConstants::Double::nan;
}



/**
 * Perform the proposal.
 *
 * A SPR proposal.
 *
 * \return The hastings ratio.
 */
double RootedUnconstrainedSBNProposal::doProposal( void )
{
// std::cout << "hello from the move" << std::endl;

    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    // Compute q(x|x') = q(x)
    stored_tree = tree->getValue();
// std::cout << "stored tree" << std::endl;

    Tree &tau = tree->getValue();

// std::cout << "dealt with trees" << std::endl;
    SBNDistribution.setValue( tree->getValue().clone() ,true);
// std::cout << "SBNDistribution.setValue(&tau,false);" << std::endl;
// Tree uhoh = SBNDistribution.getValue();

// std::cout << "SBN has set value to this tree: \n" << uhoh << std::endl;

    double lnHastingsratio = SBNDistribution.computeLnProbability();
std::cout << "Computed current tree lnProbability as " << SBNDistribution.computeLnProbability() << std::endl;

    // Store branch lengths
    for (size_t index=0; index<length; ++index)
    {
      stored_branch_lengths[index] = variables[index]->getValue();
    }
// std::cout << "stored branches" << std::endl;

    // Get new tree, compute q(x'|x) = q(x')
    SBNDistribution.redrawValue();
// std::cout << "redrew tree" << std::endl;

    lnHastingsratio -= SBNDistribution.computeLnProbability();
std::cout << "computed new tree lnProbability as " << SBNDistribution.computeLnProbability() << std::endl;

    // Set new tree
    Tree tmp = SBNDistribution.getValue();
// std::cout << "Tree tmp = SBNDistribution.getValue();" << std::endl;
// std::cout << "  tmp.getNumberOfTips() = " << tmp.getNumberOfTips() << std::endl;
// std::cout << "  tmp.getNumberOfNodes() = " << tmp.getNumberOfNodes() << std::endl;
//     std::cout << "  tmp.isBinary() = " << (tmp.isBinary() == true ? "TRUE" : "FALSE" ) << std::endl;
// std::cout << "  tmp.isBroken() = " << (tmp.isBroken() == true ? "TRUE" : "FALSE" ) << std::endl;

    // update branch lengths
    std::vector<TopologyNode*> tree_nodes = tmp.getNodes();
    size_t i=0;
    for (std::vector<TopologyNode*>::iterator it = tree_nodes.begin(); it != tree_nodes.end(); ++it)
    {
      if ( !(tree_nodes[i]->isRoot()) )
      {
        variables[i]->getValue() = (*it)->getBranchLength();
        ++i;
      }
    }

    // Update tree
    tree->getValue() = tmp;

    return lnHastingsratio;

}


/**
 *
 */
void RootedUnconstrainedSBNProposal::prepareProposal( void )
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
void RootedUnconstrainedSBNProposal::printParameterSummary(std::ostream &o, bool name_only) const
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
void RootedUnconstrainedSBNProposal::undoProposal( void )
{

  Tree &tau = tree->getValue();
  tau = stored_tree;

  for (size_t i=0; i<length; ++i)
  {
      variables[i]->getValue() = stored_branch_lengths[i];
  }


}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void RootedUnconstrainedSBNProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{

    if ( tree == oldN )
    {
        tree = static_cast<StochasticNode<Tree>* >(newN) ;
    }

    for (size_t i = 0; i < variables.size(); ++i)
    {
        if ( variables[i] == oldN )
        {
            variables[i] = static_cast<StochasticNode<double> *>(newN);
        }
    }

}


void RootedUnconstrainedSBNProposal::setProposalTuningParameter(double tp)
{
    // this proposal has no tuning parameter: nothing to do
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void RootedUnconstrainedSBNProposal::tune( double rate )
{

    // nothing to tune

}
