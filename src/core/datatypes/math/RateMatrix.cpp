/**
 * @file
 * This file contains the implementation of RateMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix
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

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix.h"
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
RateMatrix::RateMatrix(size_t n) {

    numStates            = n;
    theRateMatrix        = new MatrixReal(numStates, numStates, 0.0 );
    theStationaryFreqs   = std::vector<double>(numStates);
}



/** Copy constructor */
RateMatrix::RateMatrix(const RateMatrix& m) {

    numStates            = m.numStates;
    theRateMatrix        = new MatrixReal( *m.theRateMatrix );
    theStationaryFreqs   = m.theStationaryFreqs;
    avgRate              = m.avgRate;
    
}


/** Destructor */
RateMatrix::~RateMatrix(void) {
    
    delete theRateMatrix;
}


RateMatrix& RateMatrix::operator=(const RateMatrix &r) {
    
    if (this != &r) {
        
        delete theRateMatrix;
        
        numStates            = r.numStates;
        theRateMatrix        = new MatrixReal( *r.theRateMatrix );
        theStationaryFreqs   = r.theStationaryFreqs;
        avgRate              = r.avgRate;
        
    }
    
    return *this;
}

/** Index operator (const) */
const std::vector<double>& RateMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates )
        throw RbException( "Index to RateMatrix[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Index operator */
std::vector<double>& RateMatrix::operator[]( const size_t i ) {

    if ( i >= numStates )
        throw RbException( "Index to RateMatrix[][] out of bounds" );
    return (*theRateMatrix)[i];
}


std::vector<std::vector<double> >::const_iterator RateMatrix::begin( void ) const {
    return theRateMatrix->begin();
}


std::vector<std::vector<double> >::iterator RateMatrix::begin( void ) {
    return theRateMatrix->begin();
}


std::vector<std::vector<double> >::const_iterator RateMatrix::end( void ) const {
    return theRateMatrix->end();
}


std::vector<std::vector<double> >::iterator RateMatrix::end( void ) {
    return theRateMatrix->end();
}


/** Calculate the average rate for the rate matrix */
double RateMatrix::averageRate(void) const {

    double ave = 0.0;
    for (size_t i=0; i<numStates; i++)
        ave += -theStationaryFreqs[i] * (*theRateMatrix)[i][i];
    return ave;
}



/** This function checks that the rate matrix is time reversible. It takes as
    input the rate matrix, a, and the stationary frequencies of the process, f. 
    It checks that f[i] * q[i][j] = f[j] * q[j][i] for all i != j. It does this
    by accumulating the difference | f[i] * q[i][j] - f[j] * q[j][i] | for all
    off-diagonal comparisons. If this difference is less than tolerance,
    it reports that the rate matrix is time-reversible. If the flag isRev
    is set to true, then we do not need to check because then we have determined
    previously that the rate matrix is reversible. */ 
//bool RateMatrix::checkTimeReversibity(double tolerance) {
//	
//	double diff = 0.0;
//	for (size_t i=0; i<numStates; i++)
//		for (size_t j=i+1; j<numStates; j++)
//			diff += fabs( theStationaryFreqs[i] * (*theRateMatrix)[i][j] - theStationaryFreqs[j] * (*theRateMatrix)[j][i] );
//    reversibilityChecked = true;
//	if (diff < tolerance)
//        return true;
//	return false;
//}


size_t RateMatrix::getNumberOfStates( void ) const {
    return numStates;
}


const std::vector<double>& RateMatrix::getStationaryFrequencies( void ) const {
    return theStationaryFreqs;
}


///** Return whether or not the rate matrix is time reversible */
//bool RateMatrix::isTimeReversible(void) {
//
//    if (reversibilityChecked == false)
//        return checkTimeReversibity(0.000001);
//    return timeReversible;
//}


/** Rescale the rates such that the average rate is r */
void RateMatrix::rescaleToAverageRate(double r) {
    
    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<numStates; i++)
        for (size_t j=0; j<numStates; j++)
            (*theRateMatrix)[i][j] *= scaleFactor;
    
    
    update();
}


///** Set the diagonal of the rate matrix such that each row sums to zero */
//void RateMatrix::setDiagonal(void) {
//    
//    eigensDirty = true;
//    for (size_t i=0; i<numStates; i++)
//        {
//        double sum = 0.0;
//        for (size_t j=0; j<numStates; j++)
//            {
//            if (i != j)
//                sum += (*theRateMatrix)[i][j];
//            }
//        (*theRateMatrix)[i][i] = -sum;
//        }
//}


///** Directly set whether or not the rate matrix is time reversible */
//void RateMatrix::setTimeReversible(bool tf) {
//
//    reversibilityChecked = true;
//    timeReversible = tf;
//}


/** Set the stationary frequencies directly. We assume that we know
    what the stationary frequencies are when this function is called. We
    would know the stationary frequencies for most phylogenetic models. */
void RateMatrix::setStationaryFrequencies(const std::vector<double>& f) {

    theStationaryFreqs = f;
    update();
}



size_t RateMatrix::size( void ) const {
    return numStates;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix& x) {
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

