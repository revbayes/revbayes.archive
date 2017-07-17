#include "BetaSimplexPairProposal.h"
#include "DistributionBeta.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"
#include "TypedDagNode.h"

#include <cmath>
#include <iostream>


using namespace RevBayesCore;

/**
 * Constructor
 *
 * Here we simply allocate and initialize the Proposal object.
 */
BetaSimplexPairProposal::BetaSimplexPairProposal( StochasticNode<Simplex> *n, double a, double p ) : SimpleProposal<Simplex>( n, p ),
    storedValue( Simplex() ),
    alpha( a )
{
    // tell the base class to add the node
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void BetaSimplexPairProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
BetaSimplexPairProposal* BetaSimplexPairProposal::clone( void ) const
{
    
    return new BetaSimplexPairProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& BetaSimplexPairProposal::getProposalName( void ) const
{
    static std::string name = "BetaSimplexPair";
    
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
double BetaSimplexPairProposal::propose( Simplex &value )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // store the value
    storedValue = value;
    
    // choose the two elements
    size_t num_elements = value.size();
    size_t element_a_index = size_t( floor(rng->uniform01()*double(num_elements)) );
    size_t element_b_index = element_a_index;
    
    while ( element_b_index == element_a_index )
    {
        element_b_index = size_t( floor(rng->uniform01()*double(num_elements)) );
    }
    
    // get the current values of the elements
    double current_value_a = value[element_a_index];
    double current_value_b = value[element_b_index];
    
    // normalize the current values to sum to 1
    double current_sum                = current_value_a + current_value_b;
    double current_value_a_normalized = current_value_a / current_sum;
    double current_value_b_normalized = current_value_b / current_sum;
    
    // compute the parameters of the beta distribution
    double a_forward = (current_value_a_normalized) * alpha + 1.0;
    double b_forward = (current_value_b_normalized) * alpha + 1.0;
    
    if (a_forward < 1E-100 | b_forward < 1E-100)
    {
        // very low proposal probability which will hopefully result into a rejected proposal
        return RbConstants::Double::neginf;
    }
    
    // draw the new random varible
    double new_value = RbStatistics::Beta::rv(a_forward, b_forward, *rng);
    
    // compute the new values of the pair
    double new_value_a_normalized = new_value;
    double new_value_b_normalized = 1.0 - new_value;
    
    double new_value_a = new_value_a_normalized * current_sum;
    double new_value_b = new_value_b_normalized * current_sum;
    
    // set the values
    value[element_a_index] = new_value_a;
    value[element_b_index] = new_value_b;
    
    double ln_Hastins_ratio = 0;
    try
    {
        
        double forward    = RbStatistics::Beta::lnPdf(a_forward, b_forward, new_value);
        double a_backward = (new_value_a_normalized) * alpha + 1.0;
        double b_backward = (new_value_b_normalized) * alpha + 1.0;
        double backward   = RbStatistics::Beta::lnPdf(a_backward, b_backward, current_value_a / (current_value_a + current_value_b));
        ln_Hastins_ratio  = backward - forward;
        
    }
    catch (RbException e)
    {
        ln_Hastins_ratio = RbConstants::Double::neginf;
    }
    
    
    
    return ln_Hastins_ratio;
    
    
//    // we need to know the number of categories
//    size_t cats = value.size();
//    
//    
//    // randomly draw a new index
//    size_t chosen_index = size_t( floor(rng->uniform01()*double(cats)) );
//    double current_value = value[chosen_index];
//    
//    
//    // draw new rates and compute the hastings ratio at the same time
//    double a = alpha + 1.0;
//    double b = (a-1.0) / current_value - a + 2.0;
//    double new_value = RbStatistics::Beta::rv(a, b, *rng);
//    
//    
//    // set the value
//    value[chosen_index] = new_value;
//
//    double scaling_factor_other_values = (1.0-new_value) / (1.0-current_value);
//    
//    for(size_t i=0; i<cats; i++)
//    {
//        if ( i != chosen_index )
//        {
//            value[i] *= scaling_factor_other_values;
//        }
//    }
//    
//    double ln_Hastins_ratio = 0;
//    try
//    {
//        
//        // compute the Hastings ratio
//        double forward = RbStatistics::Beta::lnPdf(a, b, new_value);
//        double new_a = alpha + 1.0;
//        double new_b = (a-1.0) / value[chosen_index] - a + 2.0;
//        double backward = RbStatistics::Beta::lnPdf(new_a, new_b, current_value);
//        
//        ln_Hastins_ratio = backward - forward;
//    }
//    catch (RbException e)
//    {
//        ln_Hastins_ratio = RbConstants::Double::neginf;
//    }
//    
//    return ln_Hastins_ratio;
    
    
}


/**
 *
 */
void BetaSimplexPairProposal::prepareProposal( void )
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
void BetaSimplexPairProposal::printParameterSummary(std::ostream &o) const
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
void BetaSimplexPairProposal::undoProposal( void )
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
void BetaSimplexPairProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast<StochasticNode<Simplex>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void BetaSimplexPairProposal::tune( double rate )
{
    
    double p = this->targetAcceptanceRate;
    
    if ( rate > p )
    {
        alpha /= (1.0 + ((rate-p)/(1.0 - p)) );
    }
    else
    {
        alpha *= (2.0 - rate/p);
    }
    
}

