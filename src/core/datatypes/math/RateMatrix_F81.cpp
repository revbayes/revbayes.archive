/**
 * @file
 * This file contains the implementation of RateMatrix_F81, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_F81
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
 * $Id: RateMatrix_F81.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "EigenSystem.h"
#include "MatrixReal.h"
#include "RateMatrix_F81.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_F81::RateMatrix_F81(size_t n) : RateMatrix( n ){
    
    theStationaryFreqs = std::vector<double>(n,1.0/n);
}


/** Copy constructor */
RateMatrix_F81::RateMatrix_F81(const RateMatrix_F81& m) : RateMatrix( m ) {
    
}


/** Destructor */
RateMatrix_F81::~RateMatrix_F81(void) {
    
}


RateMatrix_F81& RateMatrix_F81::operator=(const RateMatrix_F81 &r) {
    
    if (this != &r) 
    {
        RateMatrix::operator=( r );
        
    }
    
    return *this;
}



/** Calculate the transition probabilities */
void RateMatrix_F81::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
    // compute auxilliary variables
    double tmp = 1.0;
	for (size_t i=0; i<numStates; i++) tmp -= theStationaryFreqs[i]*theStationaryFreqs[i];
    double beta = 1.0 / tmp; 
    double xx = -beta * t;
    double e = exp( xx );
    double oneminuse = 1.0 - e;
    
    // calculate the transition probabilities
	for (size_t i=0; i<numStates; i++) 
    {
        double to_i = theStationaryFreqs[i] * oneminuse;
		for (size_t j=0; j<numStates; j++) 
        {
            P[j][i] = to_i;    // it is easier to overwrite the case i -> i later than checking for every j
        }
         
        // from i to i
        P[i][i] = theStationaryFreqs[i] + (1.0-theStationaryFreqs[i])*e;
    }
}


RateMatrix_F81* RateMatrix_F81::clone( void ) const {
    return new RateMatrix_F81( *this );
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix_F81& x) {
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

