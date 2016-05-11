#include "CorrelationMatrixProposal.h"
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
CorrelationMatrixProposal::CorrelationMatrixProposal( StochasticNode<MatrixReal> *n, RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* s, double l ) : Proposal(),
variable( n ),
storedValue( 0.0 ),
sigma( s ),
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
void CorrelationMatrixProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
CorrelationMatrixProposal* CorrelationMatrixProposal::clone( void ) const
{
    
    return new CorrelationMatrixProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& CorrelationMatrixProposal::getProposalName( void ) const
{
    static std::string name = "CorrelationMatrixProposal";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A sliding proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and MatrixRealSingleElementSlidings the current vale by
 * delta = lambda * u
 * where lambda is the tuning parameter of the proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double CorrelationMatrixProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    MatrixReal& v = variable->getValue();
    const RbVector<double>& s = sigma->getValue();
    
    // choose an index
    indexa = size_t( rng->uniform01() * v.getNumberOfRows() );
    indexb = size_t( rng->uniform01() * v.getNumberOfColumns() );
    
    // copy value
    storedValue = v[indexa][indexb];
    
    // Generate new value (no reflection, so we simply abort later if we propose value here outside of support)
    v[indexa][indexb] = 2.0 * rng->uniform01() - 1.0;

    // check that the resulting variance-covariance matrix is positive definite




    
    variable->addTouchedElementIndex(indexa);
    variable->addTouchedElementIndex(indexb);
    
    // this is a symmetric proposal so the hasting ratio is 0.0
    return 0.0;
}


/**
 *
 */
void CorrelationMatrixProposal::prepareProposal( void )
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
void CorrelationMatrixProposal::printParameterSummary(std::ostream &o) const
{
    
    o << "delta = " << lambda;
    
}


/**
 * Reject the Proposal.
 *
 * Since the Proposal stores the previous value and it is the only place
 * where complex undo operations are known/implement, we need to revert
 * the value of the variable/DAG-node to its original value.
 */
void CorrelationMatrixProposal::undoProposal( void )
{
    
    MatrixReal& v = variable->getValue();
    v[indexa][indexb] = storedValue;
    variable->clearTouchedElementIndices();
    
}


/**
 * Swap the current variable for a new one.
 *
 * \param[in]     oldN     The old variable that needs to be replaced.
 * \param[in]     newN     The new RevVariable.
 */
void CorrelationMatrixProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
{
    
    variable = static_cast< StochasticNode<MatrixReal>* >(newN) ;
    
}


/**
 * Tune the Proposal to accept the desired acceptance ratio.
 *
 * The acceptance ratio for this Proposal should be around 0.44.
 * If it is too large, then we increase the proposal size,
 * and if it is too small, then we decrease the proposal size.
 */
void CorrelationMatrixProposal::tune( double rate )
{
    
    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }
    
    lambda = fmin(10000, lambda);
    
}

