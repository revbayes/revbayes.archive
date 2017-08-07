#include "CorrelationMatrixPartialElementBetaProposal.h"
#include "DistributionBeta.h"
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
CorrelationMatrixPartialElementBetaProposal::CorrelationMatrixPartialElementBetaProposal( StochasticNode<MatrixReal> *n, double a) : Proposal(),
    variable( n ),
    alpha( a ),
    storedValue( n->getValue().getDim() )
{
    // tell the base class to add the node
    addNode( variable );
    
    // initialize the stored value
//    storedValue = variable->getValue();
    
}


/**
 * The cleanProposal function may be called to clean up memory allocations after AbstractMove
 * decides whether to accept, reject, etc. the proposed value.
 *
 */
void CorrelationMatrixPartialElementBetaProposal::cleanProposal( void )
{
    ; // do nothing
}

/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the proposal.
 */
CorrelationMatrixPartialElementBetaProposal* CorrelationMatrixPartialElementBetaProposal::clone( void ) const
{
    
    return new CorrelationMatrixPartialElementBetaProposal( *this );
}


/**
 * Get Proposals' name of object
 *
 * \return The Proposals' name.
 */
const std::string& CorrelationMatrixPartialElementBetaProposal::getProposalName( void ) const
{
    static std::string name = "MatrixRealSingleElementBetaMove";
    
    return name;
}


/**
 * Perform the proposal.
 *
 * A Beta proposal draws a random uniform number u ~ unif(-0.5,0.5)
 * and MatrixRealSingleElementBetas the current vale by
 * delta = lambda * u
 * where lambda is the tuning parameter of the proposal to influence the size of the proposals.
 *
 * \return The hastings ratio.
 */
double CorrelationMatrixPartialElementBetaProposal::doProposal( void )
{
    
    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;
    
    // first, store the old matrix
    storedValue = variable->getValue();
    
    size_t dim = storedValue.getDim();
    
    // now, get the inverse of the matrix
    variable->getValue().setCholesky(true);
    MatrixReal inverse_matrix = variable->getValue().computeInverse();
    
    // compute the correlation matrix
    MatrixReal partial_correlations(dim, dim);

    for(size_t i = 0; i < dim; ++i)
    {
        partial_correlations[i][i] = 1.0;
        for(size_t j = i + 1; j < dim; ++j)
        {
            partial_correlations[i][j] = -inverse_matrix[i][j] / std::pow( inverse_matrix[i][i] * inverse_matrix[j][j], 0.5);
            partial_correlations[j][i] = -inverse_matrix[j][i] / std::pow( inverse_matrix[i][i] * inverse_matrix[j][j], 0.5);
        }
    }

    // choose an index
    size_t indexa = size_t( rng->uniform01() * dim );
    size_t indexb = size_t( rng->uniform01() * dim );
    
    // make sure we don't get a diagonal
    while (indexb == indexa)
    {
        indexb = size_t( rng->uniform01() * dim );
    }

    // get the chosen partial correlation
    double current_value = partial_correlations[indexa][indexb];

//    std::cout << current_value;
    // transform the current value from [-1, 1] to [0, 1]
    current_value = (current_value + 1.0) / 2.0;

    // draw new rates and compute the hastings ratio at the same time
//    double a = alpha * current_value + 1.0;
//    double b = alpha * (1.0 - current_value) + 1.0;
    double a = alpha + 10.0;
    double b = (a - 10.0) / current_value - a + 20.0;
    double new_value = RbStatistics::Beta::rv(a, b, *rng);

    // set the value (for both sides of the matrix!)
    double new_value_transformed = new_value * 2.0 - 1.0;
    partial_correlations[indexa][indexb] = new_value_transformed;
    partial_correlations[indexb][indexa] = new_value_transformed;
    
//    std::cout << " -- " << new_value_transformed;
    
    // now transform the partial correlations back into a correlation matrix
    MatrixReal P(dim, dim, 1.0);
    
    for(int k = 0; k < dim - 1; ++k)
    {
        for(int i = k + 1; i < dim; ++i)
        {
            double p = partial_correlations[k][i]; // initial value for the product-moment correlation
            for(int l = k - 1; l >= 0; --l)
            {
                p = p * pow( ( 1.0 - pow(partial_correlations[l][i], 2) ) * ( 1.0 - pow(partial_correlations[l][k], 2) ), 0.5) + partial_correlations[l][i] * partial_correlations[l][k];
            }
            P[i][k] = p;
            P[k][i] = p;
        }
        
    }

    variable->setValue( P.clone() );

    double ln_Hastings_ratio = 0.0;
    try
    {
        // compute the Hastings ratio
        double forward = RbStatistics::Beta::lnPdf(a, b, new_value);
        double new_a = alpha + 10.0;
        double new_b = (a - 10.0) / new_value - a + 20.0;
//        double new_a = alpha * new_value + 1.0;
//        double new_b = alpha * (1.0 - new_value) + 1.0;
        double backward = RbStatistics::Beta::lnPdf(new_a, new_b, current_value);
        ln_Hastings_ratio = backward - forward;
    }
    catch (RbException e)
    {
        ln_Hastings_ratio = RbConstants::Double::neginf;
    }
    
//    std::cout << " -- " << ln_Hastings_ratio << std::endl;
    
    
    return ln_Hastings_ratio;
    
}


/**
 *
 */
void CorrelationMatrixPartialElementBetaProposal::prepareProposal( void )
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
void CorrelationMatrixPartialElementBetaProposal::printParameterSummary(std::ostream &o) const
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
void CorrelationMatrixPartialElementBetaProposal::undoProposal( void )
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
void CorrelationMatrixPartialElementBetaProposal::swapNodeInternal(DagNode *oldN, DagNode *newN)
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
void CorrelationMatrixPartialElementBetaProposal::tune( double rate )
{
    
//    double p = this->targetAcceptanceRate;
//    
//    if ( rate > p )
//    {
//        alpha /= (1.0 + ((rate-p)/(1.0 - p)) );
//    }
//    else
//    {
//        alpha *= (2.0 - rate/p);
//    }
    
}

