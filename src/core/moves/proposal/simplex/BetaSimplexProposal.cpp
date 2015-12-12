#include "BetaSimplexProposal.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>

#ifdef RB_MPI
#include <mpi.h>
#endif


using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
BetaSimplexProposal::BetaSimplexProposal( StochasticNode<RbVector<double> > *n, double a ) : Proposal(),
    variable( n ),
    storedValue( RbVector<double>() ),
    alpha( a )
{
    // tell the base class to add the node
    addNode( variable );
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void BetaSimplexProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
BetaSimplexProposal* BetaSimplexProposal::clone( void ) const
{
    
    return new BetaSimplexProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& BetaSimplexProposal::getProposalName( void ) const
{
    static std::string name = "BetaSimplex";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Beta-simplex proposal randomly changes some values of a simplex, although the other values
 * change too because of the renormalization.
 * First, some random indices are drawn. Then, the proposal draws a new simplex
 *   u ~ Beta(val[index] * alpha)
 * where alpha is the tuning parameter.The new value is set to u.
 * The simplex is then renormalized.
 *
 * \return The hastings ratio.
 */
double BetaSimplexProposal::doProposal( void )
{
    
#ifdef RB_MPI
    
    size_t pid = 0;
    pid = MPI::COMM_WORLD.Get_rank();
    
    // testing
    std::stringstream ss;
    ss << "BetaSimplex_move_pid_" << pid << ".txt";
    std::string fileName = ss.str();
    std::fstream outStream;
    outStream.open( fileName.c_str(), std::fstream::out);
    
    outStream << "Starting move ..." << std::endl;
    
#endif
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // get the current value
    RbVector<double>& value = variable->getValue();
    
#ifdef RB_MPI
    
    outStream << "Old value:\t\t" << value << std::endl;
    
#endif
    
    // store the value
    storedValue = value;
    
    // we need to know the number of categories
    size_t cats = value.size();
    
#ifdef RB_MPI
    
    outStream << "#Cats:\t\t" << cats << std::endl;
    
#endif
    
    // randomly draw a new index
    size_t chosen_index = size_t( floor(rng->uniform01()*double(cats)) );
    double current_value = value[chosen_index];
    
#ifdef RB_MPI
    
    outStream << "Chosen index:\t\t" << chosen_index << std::endl;
    outStream << "Current value:\t\t" << current_value << std::endl;
    
#endif
    
    // draw new rates and compute the hastings ratio at the same time
    double a = alpha + 1.0;
    double b = (a-1.0) / current_value - a + 2.0;
    double new_value = RbStatistics::Beta::rv(a, b, *rng);
    
#ifdef RB_MPI
    
    outStream << "New Value:\t\t" << new_value << std::endl;
    
#endif
    
    // set the value
    value[chosen_index] = new_value;

    double scaling_factor_other_values = (1.0-new_value) / (1.0-current_value);
    
    // rescale
//    double sum = 0;
//    for(size_t i=0; i<cats; i++)
//    {
//        sum += value[i];
//    }
    for(size_t i=0; i<cats; i++)
    {
        if ( i != chosen_index )
        {
            value[i] *= scaling_factor_other_values;
        }
//        value[i] /= sum;
    }
    
#ifdef RB_MPI
    
    outStream << "New Values:\t\t" << value << std::endl;
    
#endif
    
    // compute the Hastings ratio
    double forward = RbStatistics::Beta::lnPdf(a, b, new_value);
    double new_a = alpha + 1.0;
    double new_b = (a-1.0) / value[chosen_index] - a + 2.0;
    double backward = RbStatistics::Beta::lnPdf(new_a, new_b, current_value);
    
#ifdef RB_MPI
    
    outStream << "P(Forward):\t\t" << forward << std::endl;
    outStream << "P(backward):\t\t" << backward << std::endl;
    outStream.close();
    
#endif
    
    return backward - forward;
}


/**
 *
 */
void BetaSimplexProposal::prepareProposal( void )
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
void BetaSimplexProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "alpha = " << alpha;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void BetaSimplexProposal::undoProposal( void )
{
    // swap current value and stored value
    variable->setValue( storedValue.clone() );
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void BetaSimplexProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<RbVector<double> >* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void BetaSimplexProposal::tune( double rate )
{
    
    if ( rate > 0.234 )
    {
        alpha /= (1.0 + ((rate-0.234)/0.766) );
    }
    else
    {
        alpha *= (2.0 - rate/0.234 );
    }
    
}

