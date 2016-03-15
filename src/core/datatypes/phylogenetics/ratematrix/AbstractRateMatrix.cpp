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
    theRateMatrix( new MatrixReal(numStates, numStates, 1.0) ),
    needsUpdate( true )
{
    
    // I cannot call a pure virtual function from the constructor (Sebastian)
    //    update();
}



/** Copy constructor */
AbstractRateMatrix::AbstractRateMatrix(const AbstractRateMatrix& m) : RateMatrix(m),
    theRateMatrix( new MatrixReal(*m.theRateMatrix) ),
    needsUpdate( true )
{
    
}


/** Destructor */
AbstractRateMatrix::~AbstractRateMatrix(void)
{
    
    delete theRateMatrix;
}


AbstractRateMatrix& AbstractRateMatrix::operator=(const AbstractRateMatrix &r)
{
    
    if (this != &r)
    {
        // delegate to parent class
        RateMatrix::operator=( r );
        
        delete theRateMatrix;
        
        
        theRateMatrix       = new MatrixReal( *r.theRateMatrix );
        needsUpdate         = true;
        
    }
    
    return *this;
}

///** Index operator (const) */
//const std::vector<double>& AbstractRateMatrix::operator[]( const size_t i ) const {
//    
//    if ( i >= numStates )
//    {
//        throw RbException( "Index to RateMatrix[][] out of bounds" );
//    }
//    
//    return (*theRateMatrix)[i];
//}
//
//
///** Index operator */
//std::vector<double>& AbstractRateMatrix::operator[]( const size_t i ) {
//    
//    if ( i >= numStates )
//    {
//        throw RbException( "Index to RateMatrix[][] out of bounds" );
//    }
//    
//    return (*theRateMatrix)[i];
//}


//std::vector<std::vector<double> >::const_iterator AbstractRateMatrix::begin( void ) const
//{
//    return theRateMatrix->begin();
//}
//
//
//std::vector<std::vector<double> >::iterator AbstractRateMatrix::begin( void )
//{
//    return theRateMatrix->begin();
//}
//
//
//std::vector<std::vector<double> >::const_iterator AbstractRateMatrix::end( void ) const
//{
//    return theRateMatrix->end();
//}
//
//
//std::vector<std::vector<double> >::iterator AbstractRateMatrix::end( void )
//{
//    return theRateMatrix->end();
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
	for (size_t i=0; i<numStates; i++)
    {
	
        for (size_t j=i+1; j<numStates; j++)
        {
			diff += fabs( theStationaryFreqs[i] * (*theRateMatrix)[i][j] - theStationaryFreqs[j] * (*theRateMatrix)[j][i] );
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
//    return numStates;
//}



double AbstractRateMatrix::getRate(size_t from, size_t to) const
{
    if ( from >= numStates || to > numStates )
    {
        throw RbException( "Index to RateMatrix.getRate() out of bounds" );
    }
    
    return (*theRateMatrix)[from][to];
}


/** Rescale the rates such that the average rate is r */
void AbstractRateMatrix::rescaleToAverageRate(double r)
{
    
    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<numStates; i++)
    {
        for (size_t j=0; j<numStates; j++)
        {
            (*theRateMatrix)[i][j] *= scaleFactor;
        }
    }
    
    // set flags
    needsUpdate = true;
    
}


/** Set the diagonal of the rate matrix such that each row sums to zero */
void AbstractRateMatrix::setDiagonal(void)
{
    
    for (size_t i=0; i<numStates; i++)
    {
        double sum = 0.0;
        for (size_t j=0; j<numStates; j++)
        {
            
            if (i != j)
            {
                sum += (*theRateMatrix)[i][j];
            }
            
        }
        (*theRateMatrix)[i][i] = -sum;
    }
    
    // set flags
    needsUpdate = true;
}



//size_t AbstractRateMatrix::size( void ) const
//{
//    return numStates;
//}

