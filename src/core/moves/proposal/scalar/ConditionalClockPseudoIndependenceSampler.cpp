#include "ConditionalClockPseudoIndependenceSampler.h"
#include "DistributionNormal.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
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
ConditionalClockISProposal::ConditionalClockISProposal( StochasticNode<double>* n, StochasticNode<Tree> *tr, double s, int k, int N, bool l ) : Proposal(),
    variable( n ),
    tree ( tr ),
    sigma ( s ),
    nInv ( k ),
    nSites ( N ),
    isLogScale ( l ),
    storedValue( 0.0 )
{
    // tell the base class to add the node
    addNode( variable );
    addNode( tree );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void ConditionalClockISProposal::cleanProposal( void )
{
    
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
ConditionalClockISProposal* ConditionalClockISProposal::clone( void ) const
{
    
    return new ConditionalClockISProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& ConditionalClockISProposal::getProposalName( void ) const
{
    static std::string name = "ConditionalClockIS";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A scaling Proposal draws a random uniform number u ~ unif (-0.5,0.5)
 * and scales the current vale by a scaling factor
 * sf = exp( sigma * u )
 * where sigma is the tuning parameter of the Proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double ConditionalClockISProposal::doProposal( void )
{
    
//    std::cout << "PROPOSING proposal on clock rate" <<std::endl;

    // Get current value
    double &val = variable->getValue();
//    std::cout << "old clock rate (variable): " << val <<std::endl;
    
    // copy value
    storedValue = val;
    
    // Get the clock rate (on the non-log scale) for calculations regarding tree length (in substitutions)
    if ( isLogScale ) {
        val = std::exp(val);
    }
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    Tree& tau = tree->getValue();
    double treeLength = tau.getTreeLength();
    
//    std::cout << "current tree length (time): " << treeLength <<std::endl;
    // What we really have is a pseudo-independence sampler on the tree length in expected substitutions per site
    double logSubsTL = std::log(val * treeLength);
    
//    std::cout << "current tree length (substitutions): " << std::exp(logSubsTL) <<std::endl;
    
    double logMean = std::log(-std::log( (nInv + 0.5) / (nSites + 0.5)));

//    std::cout << "distro median: " << std::exp(logMean) <<std::endl;
    
    // draw new substitution tree length
    double new_logSubsTL = RbStatistics::Normal::rv(*GLOBAL_RNG) * sigma + logMean;
    
    // compute the hastings ratio at the same time (easy since kernel is independent of current state)
    double probNew = RbStatistics::Normal::lnPdf(logMean, sigma, new_logSubsTL);
    double probOld = RbStatistics::Normal::lnPdf(logMean, sigma, logSubsTL);
    double lnHastingsratio = probOld - probNew;
    
    // transform tree length into clock rate
    double newVal = std::exp(new_logSubsTL)/treeLength;
    if ( isLogScale ) {
        newVal = std::log(newVal);
    }
    
//    std::cout << "old clock rate: " << val <<std::endl;
    // Set variable value to new value
    variable->setValue( new double(newVal) );
//    double &the_val = variable->getValue();
//    the_val = newVal;
//    variable->getValue() = newVal;
    
//    std::cout << "new clock rate: " << newVal <<std::endl;
    
//    std::cout << "lnHastingsratio: " << lnHastingsratio <<std::endl;
//    std::cout << "PROPOSED" <<std::endl;

    return lnHastingsratio;
}


/**
 * Prepare the proposal, e.g., pick the element that we want to change.
 * Here we do not need to do any preparation.
 */
void ConditionalClockISProposal::prepareProposal( void )
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
void ConditionalClockISProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "sigma = " << sigma;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void ConditionalClockISProposal::undoProposal( void )
{
//    std::cout << "UNDOING proposal on clock rate" <<std::endl;
//    std::cout << "old clock rate (variable): " << variable->getValue() <<std::endl;
    variable->setValue( new double(storedValue) );
//    std::cout << "new clock rate (variable): " << variable->getValue() <<std::endl;
//    std::cout << "UNDONE" <<std::endl;
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void ConditionalClockISProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    if ( oldN == variable )
    {
        variable = static_cast<StochasticNode<double>* >(newN);
    }
    else if ( oldN == tree )
    {
        tree = static_cast<StochasticNode<Tree>* >(newN);
    }
    
}

/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void ConditionalClockISProposal::tune( double rate )
{
    
    if ( rate > 0.6 )
    {
        sigma *= (1.0 + ((rate-0.6)/0.77) );
    }
    else
    {
        sigma /= (2.0 - rate/0.6 );
    }
    
}

