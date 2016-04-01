#include "MatrixReal.h"
#include "AbstractRateMatrix.h"
#include "RbException.h"
#include "RbMathMatrix.h"
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


//size_t AbstractRateMatrix::getNumberOfStates( void ) const
//{
//    return num_states;
//}



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

