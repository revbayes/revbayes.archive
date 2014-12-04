/**
 * @file
 * This file contains the implementation of RateMatrix_Mk1, which is a
 * class that holds a rate matrix for the Markov k-state 1 parameter model. 
 * Parameters:
 * k states
 * lambda: rate
 *
 * @brief Implementation of RateMatrix_Mk1
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */

#include "RateMatrix_Mk1.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Mk1::RateMatrix_Mk1(size_t n) : AbstractRateMatrix( n ), matrixSize( n ){
    setLambda(0.0);
    updateMatrix();
}


/** Copy constructor */
RateMatrix_Mk1::RateMatrix_Mk1(const RateMatrix_Mk1& m) : AbstractRateMatrix( m ), matrixSize(m.matrixSize), lambda(m.lambda), precision(m.precision), stationaryFreqs(m.stationaryFreqs) {
	
}


/** Destructor */
RateMatrix_Mk1::~RateMatrix_Mk1(void) {
    
}


RateMatrix_Mk1& RateMatrix_Mk1::operator=(const RateMatrix_Mk1 &r) {
    
    if (this != &r) {
        RateMatrix::operator=( r );
    }
    
    return *this;
}

double RateMatrix_Mk1::averageRate(void) const {
    return 1.0;
}

void RateMatrix_Mk1::setLambda( double l ) {
	
	lambda = l;
	
	// set flags
	needsUpdate = true;
	
}



void RateMatrix_Mk1::buildRateMatrix(void) 
{
    
    for (size_t i=0; i< matrixSize; i++) {
        for (size_t j=0; j< matrixSize; j++) {
            if (j != i) {
                (*theRateMatrix)[i][j] = lambda;
			}
        }
    }
    // set the diagonal values
    setDiagonal();

}




/** Calculate the transition probabilities */
void RateMatrix_Mk1::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
	// calculate the transition probabilities
    double bf = 1.0 / numStates;
    double oneMinusBf = 1.0 - bf;
    double p_ii = bf + oneMinusBf * exp(-(t * lambda)/oneMinusBf);
    double p_ij = bf - bf * exp(-(t * lambda)/oneMinusBf);
	for (size_t i=0; i<numStates; i++) 
    {
        P[i][i] = p_ii;
		for (size_t j=i+1; j<numStates; j++) 
        {
            P[i][j] = p_ij;
            P[j][i] = p_ij;
        }
    }
    
    return;
}




RateMatrix_Mk1* RateMatrix_Mk1::clone( void ) const {
    return new RateMatrix_Mk1( *this );
}


const std::vector<double>& RateMatrix_Mk1::getStationaryFrequencies( void ) const {
    
    return stationaryFreqs;
}


void RateMatrix_Mk1::updateMatrix( void ) {
    
    if ( needsUpdate )
    {
        buildRateMatrix();
        // clean flags
        needsUpdate = false;
    }
}


