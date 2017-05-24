#include "AbstractRateMatrix.h"
#include "MatrixReal.h"
#include "RandomNumberGenerator.h"
#include "RandomNumberFactory.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "DistributionPoisson.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
AbstractRateMatrix::AbstractRateMatrix(size_t n) : RateMatrix(n),
    the_rate_matrix( new MatrixReal(num_states, num_states, 1.0) ),
    needs_update( true )
{
    
    // I cannot call a pure virtual function from the constructor (Sebastian)
    //    update();
}



/** Copy constructor */
AbstractRateMatrix::AbstractRateMatrix(const AbstractRateMatrix& m) : RateMatrix(m),
    the_rate_matrix( new MatrixReal(*m.the_rate_matrix) ),
    needs_update( true )
{
    
}


/** Destructor */
AbstractRateMatrix::~AbstractRateMatrix(void)
{
    
    delete the_rate_matrix;
}


AbstractRateMatrix& AbstractRateMatrix::operator=(const AbstractRateMatrix &r)
{
    
    if (this != &r)
    {
        // delegate to parent class
        RateMatrix::operator=( r );
        
        delete the_rate_matrix;
        
        the_rate_matrix       = new MatrixReal( *r.the_rate_matrix );
        needs_update         = true;
        
    }
    
    return *this;
}

///** Index operator (const) */
//const std::vector<double>& AbstractRateMatrix::operator[]( const size_t i ) const {
//    
//    if ( i >= num_states )
//    {
//        throw RbException( "Index to RateMatrix[][] out of bounds" );
//    }
//    
//    return (*the_rate_matrix)[i];
//}
//
//
///** Index operator */
//std::vector<double>& AbstractRateMatrix::operator[]( const size_t i ) {
//    
//    if ( i >= num_states )
//    {
//        throw RbException( "Index to RateMatrix[][] out of bounds" );
//    }
//    
//    return (*the_rate_matrix)[i];
//}


//std::vector<std::vector<double> >::const_iterator AbstractRateMatrix::begin( void ) const
//{
//    return the_rate_matrix->begin();
//}
//
//
//std::vector<std::vector<double> >::iterator AbstractRateMatrix::begin( void )
//{
//    return the_rate_matrix->begin();
//}
//
//
//std::vector<std::vector<double> >::const_iterator AbstractRateMatrix::end( void ) const
//{
//    return the_rate_matrix->end();
//}
//
//
//std::vector<std::vector<double> >::iterator AbstractRateMatrix::end( void )
//{
//    return the_rate_matrix->end();
//}


/** This function calculates the stationary frequencies of the rate matrix. The
 rate matrix, Q, is the infinitesimal generator of the Markov chain. It is an
 n X n matrix whose off-diagonal elements are q_ij >= 0 and whose diagonal elements
 are specified such that each row sums to zero. The rate matrix is finite (has
 a fixed number of states) and we assume that the input matrix is irreducible, as
 is the usual case for substitution models. Because Q is irreducible and finite,
 it has a stationary distribution, pi, which is a row vector of n probabilities.
 The stationary probabilities can be calculated by solving the homogeneous system
 of equations, pi*Q = 0, where 0 is a vector of zeros.
 
 We do the following to calculate the stationary frequencies.
 
 1. We perform an LU decomposition of the transpose of the matrix Q.
 
 Q' = LU
 
 2. Now we set Ux = z (x will eventually hold the stationary probabilities).
 Because L is nonsingular, we have z = 0. We proceed to back substitute on
 Ux = z = 0. When u_nn = 0, we can put in any solution for x. Here, we put
 in x_n = 1. We then solve the other values of x through back substitution.
 
 3. The solution obtained in 2 is not a probability vector. We normalize the
 vector such that the sum of the elements is 1.
 
 Note that the only time we need to use this function is when we don't
 know the stationary frequencies of the rate matrix beforehand. For most
 substitution models used in molecular evolution, the stationary frequencies
 are built into the rate matrix itself. These models are time-reversible.
 This function is useful for the non-reversible models.
 
 For more information on the fascinating topic of calculating the stationary
 probabilities of a rate matrix, see:
 
 Stewart, W. J. 1999. Numerical methods for computing stationary distributions of
 finite irreducible Markov chains. In "Advances in Computational
 Probability", W. Grassmann, ed. Kluwer Academic Publishers. */
std::vector<double> AbstractRateMatrix::calculateStationaryFrequencies(void) const
{
    
    // transpose the rate matrix and put into QT
    MatrixReal QT(num_states, num_states);
    for (size_t i=0; i<num_states; i++)
    {
        for (size_t j=0; j<num_states; j++)
        {
            QT[i][j] = (*the_rate_matrix)[j][i];
        }
    }
    
    // compute the LU decomposition of the transposed rate matrix
    MatrixReal L(num_states, num_states);
    MatrixReal U(num_states, num_states);
    RbMath::computeLandU(QT, L, U);
    
    // back substitute into z = 0 to find un-normalized stationary frequencies, starting with x_n = 1.0
    std::vector<double> pi(num_states, 0.0);
    pi[num_states-1] = 1.0;
    size_t i=num_states-1;
    while ( i > 0 )
    {
        i--;
        double dotProduct = 0.0;
        for (size_t j=i+1; j<num_states; j++)
        {
            dotProduct += U[i][j] * pi[j];
        }
        pi[i] = (0.0 - dotProduct) / U[i][i];
    }
    
    // normalize the solution vector
    double sum = 0.0;
    for (size_t i=0; i<num_states; i++)
    {
        sum += pi[i];
    }
    
    for (size_t i=0; i<num_states; i++)
    {
        pi[i] /= sum;
    }
    
    // return the stationary frequencies
    return pi;
}



/** This function checks that the rate matrix is time reversible. It takes as
 input the rate matrix, a, and the stationary frequencies of the process, f.
 It checks that f[i] * q[i][j] = f[j] * q[j][i] for all i != j. It does this
 by accumulating the difference | f[i] * q[i][j] - f[j] * q[j][i] | for all
 off-diagonal comparisons. If this difference is less than tolerance,
 it reports that the rate matrix is time-reversible. If the flag isRev
 is set to true, then we do not need to check because then we have determined
 previously that the rate matrix is reversible. */
bool AbstractRateMatrix::checkTimeReversibity(double tolerance)
{
	
    std::vector<double> theStationaryFreqs = getStationaryFrequencies();
	double diff = 0.0;
	for (size_t i=0; i<num_states; i++)
    {
	
        for (size_t j=i+1; j<num_states; j++)
        {
			diff += fabs( theStationaryFreqs[i] * (*the_rate_matrix)[i][j] - theStationaryFreqs[j] * (*the_rate_matrix)[j][i] );
        }
    
    }
    //    reversibilityChecked = true;
	if (diff < tolerance)
    {
        return true;
    }
    
	return false;
}


double AbstractRateMatrix::getDominatingRate(void) const
{
    return dominating_rate;
}

void AbstractRateMatrix::computeDominatingRate(void)
{
    dominating_rate = 0.0;
    for (size_t i = 0; i < num_states; i++)
    {
        double r = -(*the_rate_matrix)[i][i];
        if ( r > dominating_rate)
        {
            dominating_rate = r;
        }
    }
}

double AbstractRateMatrix::getRate(size_t from, size_t to, double rate) const
{
    if ( from >= num_states || to > num_states )
    {
        throw RbException( "Index to RateMatrix.getRate() out of bounds" );
    }
    
    return (*the_rate_matrix)[from][to] * rate;
}



double AbstractRateMatrix::getRate(size_t from, size_t to, double age, double rate) const
{
    if ( from >= num_states || to > num_states )
    {
        throw RbException( "Index to RateMatrix.getRate() out of bounds" );
    }

    return (*the_rate_matrix)[from][to] * rate;
}

MatrixReal AbstractRateMatrix::getRateMatrix() const
{
    return *the_rate_matrix;
}

MatrixReal AbstractRateMatrix::getStochasticMatrix(size_t n)
{
    while (stochastic_matrix.size() < (n-1)) {
        computeStochasticMatrix(stochastic_matrix.size());
    }
    return stochastic_matrix[n];
}

void AbstractRateMatrix::computeStochasticMatrix(size_t n)
{
    MatrixReal r;
    if (n == 0) {
        // identity matrix, R^0
        r = MatrixReal(num_states, num_states);
        for (size_t i = 0; i < num_states; i++) {
            r[i][i] = 1.0;
        }
    }
    else if (n == 1) {
        // stochastic matrix, R^1
        r = (*the_rate_matrix) * (1.0/dominating_rate) + stochastic_matrix[0];
    }
    else {
        // stochastic matrix, R^n = R^(n-1) * R^1
        r = stochastic_matrix[n-1] * stochastic_matrix[1];
    }
    stochastic_matrix.push_back(r);
}

/** Rescale the rates such that the average rate is r */
void AbstractRateMatrix::rescaleToAverageRate(double r)
{
    
    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<num_states; i++)
    {
        for (size_t j=0; j<num_states; j++)
        {
            (*the_rate_matrix)[i][j] *= scaleFactor;
        }
    }
    
    // set flags
    needs_update = true;
    
}


void AbstractRateMatrix::simulateStochasticMapping(double startAge, double endAge, double rate,std::vector<size_t>& transition_states, std::vector<double>& transition_times)
{
    // start and end states
    size_t start_state = transition_states[0];
    size_t end_state = transition_states[1];
    double branch_length = (startAge - endAge);
    
    // transition probabilities
    TransitionProbabilityMatrix P(num_states);
    calculateTransitionProbabilities(startAge, endAge, rate, P);
    stochastic_matrix = std::vector<MatrixReal>();
    
    // dominating rate
    computeDominatingRate();
    
    // sample number of events
    size_t num_events = 0;
    double lambda = branch_length * rate * dominating_rate;
    double prob_transition_ctmc = P[start_state][end_state];
    double u = GLOBAL_RNG->uniform01();
    double g = prob_transition_ctmc * u;
    double prob_num_events_sum = 0.0;
    double prob_total_sum = 0.0;
    while (g > 0.0) {
        
        // probability for num_events
        double prob_num_events = RbStatistics::Poisson::pdf(lambda, (int)num_events);
        prob_num_events_sum += prob_num_events;
        
        // add the R^n stochastic matrix
        computeStochasticMatrix(num_events);
        
        // probability of start_state -> end_state after num_events
        double prob_transition_dtmc = stochastic_matrix[num_events][start_state][end_state];
        
        // update sampling prob
        double prob_total = prob_num_events * prob_transition_dtmc;
        
        prob_total_sum += prob_total;
        g -= prob_total;
        if (g <= 0.0)
        {
            break;
        }
        
        if (num_events > 20) {
            throw RbException("AbstractRateMatrix::simulateStochasticMapping fails to converge when sampling num_events!");
        }
        
        num_events += 1;
        
    }
    
    // sample event types per interval
    transition_states = std::vector<size_t>(1, start_state);
    for (size_t n = 0; n < num_events; n++)
    {
        size_t prev_state = transition_states[n];
        size_t num_events_left = num_events - n - 1;

        // get the normalization constant for sampling
        double p_sum = 0.0;
        for (size_t j = 0; j < num_states; j++)
        {
            p_sum += stochastic_matrix[1][prev_state][j] * stochastic_matrix[num_events_left][j][end_state];
        }
        
        // sample transition to next state
        size_t next_state = 0;
        double u = GLOBAL_RNG->uniform01() * p_sum;
        while (u > 0) {
            
            double p = stochastic_matrix[1][prev_state][next_state] * stochastic_matrix[num_events_left][next_state][end_state];
            u -= p;
            if (u <= 0) {
                break;
            }
            next_state += 1;
        }
        
        // sample the next state for the n+1 event
        transition_states.push_back(next_state);
        
    }
    
    // sample event times
    for (size_t i = 0; i < num_events; i++)
    {
        transition_times.push_back(GLOBAL_RNG->uniform01() * branch_length);
    }
    transition_times.push_back(0.0);
    std::sort( transition_times.begin(), transition_times.end() );

    
    // filter out the virtual events
    std::vector<size_t> save_states(1, transition_states[0]);
    std::vector<double> save_times(1, 0.0);
    size_t prev_state = transition_states[0];
    for (size_t i = 1; i < transition_states.size(); i++) {
        if (prev_state != transition_states[i]) {
            save_states.push_back(transition_states[i]);
            save_times.push_back(transition_times[i]);
            prev_state = transition_states[i];
        }
    }
    transition_states = save_states;
    transition_times = save_times;
    
    // convert relative times along branches into incremental times
    double sum_transition_times = 0.0;
    for (size_t i = 0; i < transition_times.size()-1; i++)
    {
        transition_times[i] = transition_times[i+1] - sum_transition_times;
        sum_transition_times += transition_times[i];
    }
    transition_times[transition_times.size()-1] = branch_length - sum_transition_times;
    
    // done!
    return;
}





/** Set the diagonal of the rate matrix such that each row sums to zero */
void AbstractRateMatrix::setDiagonal(void)
{
    
    for (size_t i=0; i<num_states; ++i)
    {
        double sum = 0.0;
        for (size_t j=0; j<num_states; ++j)
        {
            
            if (i != j)
            {
                sum += (*the_rate_matrix)[i][j];
            }
            
        }
        (*the_rate_matrix)[i][i] = -sum;
    }
    
    // set flags
    needs_update = true;
}

