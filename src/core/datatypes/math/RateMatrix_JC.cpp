/**
 * @file
 * This file contains the implementation of RateMatrix_JC, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_JC
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-12-11 14:46:24 +0100 (Tue, 11 Dec 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: RateMatrix_JC.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_JC.h"
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
RateMatrix_JC::RateMatrix_JC(size_t n) : RateMatrix( n ){
    
    theStationaryFreqs = std::vector<double>(n,1.0/n);
}


/** Copy constructor */
RateMatrix_JC::RateMatrix_JC(const RateMatrix_JC& m) : RateMatrix( m ) {
    
}


/** Destructor */
RateMatrix_JC::~RateMatrix_JC(void) {
    
}


RateMatrix_JC& RateMatrix_JC::operator=(const RateMatrix_JC &r) {
    
    if (this != &r) {
        RateMatrix::operator=( r );
                
    }
    
    return *this;
}



/** Calculate the transition probabilities */
void RateMatrix_JC::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
    // calculate the transition probabilities
    double p_ii = 1.0/numStates + (1.0-1.0/numStates) * exp(-t);
    double p_ij = 1.0/numStates - 1.0/numStates * exp(-t);
	for (size_t i=0; i<numStates; i++) {
        P[i][i] = p_ii;
		for (size_t j=i+1; j<numStates; j++) {
            P[i][j] = p_ij;
            P[j][i] = p_ij;
        }
    }
}


RateMatrix_JC* RateMatrix_JC::clone( void ) const {
    return new RateMatrix_JC( *this );
}



/** Return whether or not the rate matrix is time reversible */
bool RateMatrix_JC::isTimeReversible(void) {
    
    return true;
}


///** Directly set whether or not the rate matrix is time reversible */
//void RateMatrix_JC::setTimeReversible(bool tf) {
//    // nothing to do
//}


void RateMatrix_JC::update( void ) {
    // nothing to do
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix_JC& x) {
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

