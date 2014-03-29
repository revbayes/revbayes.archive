/**
 * @file
 * This file contains the implementation of RateValueMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateValueMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#include "MatrixReal.h"
#include "RateValueMatrix.h"
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
RateValueMatrix::RateValueMatrix(size_t n) : needsUpdate( true ) {
    
    numStates            = n;
    theRateMatrix        = new MatrixReal(numStates, numStates, 1.0 );
    
    // I cannot call a pure virtual function from the constructor (Sebastian)
    //    updateMatrix();
}



/** Copy constructor */
RateValueMatrix::RateValueMatrix(const RateValueMatrix& m) : needsUpdate( true ){
    
    numStates            = m.numStates;
    theRateMatrix        = new MatrixReal( *m.theRateMatrix );
    
}


/** Destructor */
RateValueMatrix::~RateValueMatrix(void) {
    
    delete theRateMatrix;
}


RateValueMatrix& RateValueMatrix::operator=(const RateValueMatrix &r) {
    
    if (this != &r) {
        
        delete theRateMatrix;
        
        numStates           = r.numStates;
        theRateMatrix       = new MatrixReal( *r.theRateMatrix );
        needsUpdate         = true;
        
    }
    
    return *this;
}

/** Index operator (const) */
const std::vector<double>& RateValueMatrix::operator[]( const size_t i ) const {
    
    if ( i >= numStates )
        throw RbException( "Index to RateValueMatrix[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Index operator */
std::vector<double>& RateValueMatrix::operator[]( const size_t i ) {
    
    if ( i >= numStates )
        throw RbException( "Index to RateValueMatrix[][] out of bounds" );
    return (*theRateMatrix)[i];
}

double RateValueMatrix::getRate(size_t i, size_t j) const {
    return (*theRateMatrix)[i][j];
}


std::vector<std::vector<double> >::const_iterator RateValueMatrix::begin( void ) const {
    return theRateMatrix->begin();
}


std::vector<std::vector<double> >::iterator RateValueMatrix::begin( void ) {
    return theRateMatrix->begin();
}


std::vector<std::vector<double> >::const_iterator RateValueMatrix::end( void ) const {
    return theRateMatrix->end();
}


std::vector<std::vector<double> >::iterator RateValueMatrix::end( void ) {
    return theRateMatrix->end();
}



/** This function checks that the rate matrix is time reversible. It takes as
 input the rate matrix, a, and the stationary frequencies of the process, f.
 It checks that f[i] * q[i][j] = f[j] * q[j][i] for all i != j. It does this
 by accumulating the difference | f[i] * q[i][j] - f[j] * q[j][i] | for all
 off-diagonal comparisons. If this difference is less than tolerance,
 it reports that the rate matrix is time-reversible. If the flag isRev
 is set to true, then we do not need to check because then we have determined
 previously that the rate matrix is reversible. */
bool RateValueMatrix::checkTimeReversibity(double tolerance) {
	
    std::vector<double> theStationaryFreqs = getStationaryFrequencies();
	double diff = 0.0;
	for (size_t i=0; i<numStates; i++)
		for (size_t j=i+1; j<numStates; j++)
			diff += fabs( theStationaryFreqs[i] * (*theRateMatrix)[i][j] - theStationaryFreqs[j] * (*theRateMatrix)[j][i] );
    //    reversibilityChecked = true;
	if (diff < tolerance)
        return true;
    
	return false;
}


size_t RateValueMatrix::getNumberOfStates( void ) const {
    return numStates;
}


///** Return whether or not the rate matrix is time reversible */
//bool RateValueMatrix::isTimeReversible(void) {
//
//    if (reversibilityChecked == false)
//        return checkTimeReversibity(0.000001);
//    return timeReversible;
//}


/** Rescale the rates such that the average rate is r */
void RateValueMatrix::rescaleToAverageRate(double r) {
    
    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<numStates; i++)
        for (size_t j=0; j<numStates; j++)
            (*theRateMatrix)[i][j] *= scaleFactor;
    
    // set flags
    needsUpdate = true;
    
}


/** Set the diagonal of the rate matrix such that each row sums to zero */
void RateValueMatrix::setDiagonal(void) {
    
    for (size_t i=0; i<numStates; i++)
    {
        double sum = 0.0;
        for (size_t j=0; j<numStates; j++)
        {
            if (i != j)
                sum += (*theRateMatrix)[i][j];
        }
        (*theRateMatrix)[i][i] = -sum;
    }
    
    // set flags
    needsUpdate = true;
}



size_t RateValueMatrix::size( void ) const {
    return numStates;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateValueMatrix& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j)
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}

