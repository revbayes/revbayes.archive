#include "DistributionBeta.h"
#include "LogRandomDiveProposal.h"
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
LogRandomDiveProposal::LogRandomDiveProposal( StochasticNode<double> *n, double l, double p) : Proposal(p),
    variable( n ),
    storedValue( 0.0 ),
    lambda( l )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void LogRandomDiveProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
LogRandomDiveProposal* LogRandomDiveProposal::clone( void ) const
{
    
    return new LogRandomDiveProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& LogRandomDiveProposal::getProposalName( void ) const
{
    static std::string name = "LogRandomDive";
    
    return name;
}


double LogRandomDiveProposal::getProposalTuningParameter( void ) const
{
    return lambda;
}


/**
 * Perform the proposal.
 *
 * A Random Dive proposal on the log-scale of a variable.
 *
 * \return The hastings ratio.
 */
double LogRandomDiveProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    double &val = variable->getValue();
    
    // copy value
    storedValue = val;
    
    // log value
    val = log(val);
    
    // Draw from a stretched and translated Beta(lambda,lambda) - {0} distribution that does not include -1 or 1
    double epsilon  = ( 2 * RbStatistics::Beta::rv(lambda, lambda, *rng) ) - 1.0 ;
    while ( epsilon == 0.0 || epsilon == 1.0 || epsilon == -1.0 )
    {
        epsilon  = ( 2 * RbStatistics::Beta::rv(lambda, lambda, *rng) ) - 1.0 ;
    }
    
    double u = rng->uniform01();
    
    if (u >= 0.5)
    {
        epsilon = 1/epsilon;
    }
    
    double hr = log(fabs(epsilon));
    
    double newVal = val * epsilon;
    
    hr += newVal;
    hr -= val;
    
    val = exp(newVal);
    
    return hr;
}


/**
 *
 */
void LogRandomDiveProposal::prepareProposal( void )
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
void LogRandomDiveProposal::printParameterSummary(std::ostream &o, bool name_only) const
{
    
    o << "delta = ";
    if (name_only == false)
    {
        o << lambda;
    }
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void LogRandomDiveProposal::undoProposal( void )
{
    // swap current value and stored value
    variable->setValue( new double(storedValue) );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void LogRandomDiveProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast< ContinuousStochasticNode* >(newN) ;
    
}


void LogRandomDiveProposal::setProposalTuningParameter(double tp)
{
    lambda = tp;
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void LogRandomDiveProposal::tune( double rate )
{
    // As the proposal distribution gets more concentrated around 0, the proposals get bolder
    double p = this->targetAcceptanceRate;
    if ( rate > p )
    {
        lambda *= (1.0 + ((rate-p)/(1.0 - p)) );
    }
    else
    {
        lambda /= (2.0 - rate/p);
    }
    
    lambda = fmin(10000, lambda);
    
}

