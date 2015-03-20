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
RateMatrix_F81::RateMatrix_F81(size_t n) : TimeReversibleRateMatrix( n ){
    
    update();
}


/** Destructor */
RateMatrix_F81::~RateMatrix_F81(void) {
    
}



/** Calculate the transition probabilities */
void RateMatrix_F81::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    // compute auxilliary variables
    double t = rate * (startAge - endAge);
    double tmp = 1.0;
	for (size_t i=0; i<numStates; i++) tmp -= stationaryFreqs[i]*stationaryFreqs[i];
    double beta = 1.0 / tmp; 
    double xx = -beta * t;
    double e = exp( xx );
    double oneminuse = 1.0 - e;
    
    // calculate the transition probabilities
	for (size_t i=0; i<numStates; i++) 
    {
        double to_i = stationaryFreqs[i] * oneminuse;
		for (size_t j=0; j<numStates; j++) 
        {
            P[j][i] = to_i;    // it is easier to overwrite the case i -> i later than checking for every j
        }
         
        // from i to i
        P[i][i] = stationaryFreqs[i] + (1.0-stationaryFreqs[i])*e;
    }
}


RateMatrix_F81* RateMatrix_F81::clone( void ) const {
    return new RateMatrix_F81( *this );
}


void RateMatrix_F81::update( void ) {
    
    if ( needsUpdate ) 
    {
        // @todo: This is only needed for printing the values of the rate matrix properly to the screen. We should do this more efficiently (Sebastian).
        // We could instead only update the matrix if a print call happened and the matrix was flagged as dirty.
        
        // compute the off-diagonal values
        computeOffDiagonal();
        
        // set the diagonal values
        setDiagonal();
        
        // rescale 
        rescaleToAverageRate( 1.0 );
        
        // clean flags
        needsUpdate = false;
    }
}





