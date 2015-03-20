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
RateMatrix_JC::RateMatrix_JC(size_t n) : TimeReversibleRateMatrix( n )
{
    
    
    // compute the off-diagonal values
    computeOffDiagonal();
            
    // set the diagonal values
    setDiagonal();
            
    // rescale 
    rescaleToAverageRate( 1.0 );
            
}


/** Destructor */
RateMatrix_JC::~RateMatrix_JC(void) {
    
}



/** Calculate the transition probabilities */
void RateMatrix_JC::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    
    double t = rate * (startAge - endAge);
    
    // calculate the transition probabilities
    double bf = 1.0 / numStates;
    double oneMinusBf = 1.0 - bf;
    double p_ii = bf + oneMinusBf * exp(-t/oneMinusBf);
    double p_ij = bf - bf * exp(-t/oneMinusBf);
	for (size_t i=0; i<numStates; i++) 
    {
        P[i][i] = p_ii;
		for (size_t j=i+1; j<numStates; j++) 
        {
            P[i][j] = p_ij;
            P[j][i] = p_ij;
        }
    }
}


RateMatrix_JC* RateMatrix_JC::clone( void ) const {
    return new RateMatrix_JC( *this );
}


void RateMatrix_JC::update( void ) {
    // nothing to do
}

