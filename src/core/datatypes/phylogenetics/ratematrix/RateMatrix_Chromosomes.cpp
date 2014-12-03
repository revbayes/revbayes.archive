/**
 * @file
 * This file contains the implementation of RateMatrix_Chromosomes, which is a
 * class that holds a rate matrix for the chromosome number evolution model. 
 *
 * @brief Implementation of RateMatrix_Chromosomes
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */

#include "RateMatrix_Chromosomes.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Chromosomes::RateMatrix_Chromosomes(size_t n) : AbstractRateMatrix( n ), matrixSize( n ){
    setLambda(0.0);
    setRho(0.0);
    setDelta(0.0);
    updateMatrix();
}


/** Copy constructor */
RateMatrix_Chromosomes::RateMatrix_Chromosomes(const RateMatrix_Chromosomes& m) : AbstractRateMatrix( m ), matrixSize(m.matrixSize), rho(m.rho), delta(m.delta), lambda(m.lambda), precision(m.precision), stationaryFreqs(m.stationaryFreqs) {

}


/** Destructor */
RateMatrix_Chromosomes::~RateMatrix_Chromosomes(void) {
    
}


RateMatrix_Chromosomes& RateMatrix_Chromosomes::operator=(const RateMatrix_Chromosomes &r) {
    
    if (this != &r) {
        RateMatrix::operator=( r );
    }
    
    return *this;
}

double RateMatrix_Chromosomes::averageRate(void) const {
    return 1.0;
}

void RateMatrix_Chromosomes::setLambda( double l ) {

        lambda = l;

        // set flags
        needsUpdate = true;

}

void RateMatrix_Chromosomes::setRho( double r ) {

        rho = r;

        // set flags
        needsUpdate = true;

}

void RateMatrix_Chromosomes::setDelta( double d ) {

        delta = d;

        // set flags
        needsUpdate = true;

}


void RateMatrix_Chromosomes::buildRateMatrix(void) 
{
    
    for (size_t i=0; i< matrixSize; i++) {
        for (size_t j=0; j< matrixSize; j++) {
            if (j == i+1) {
                (*theRateMatrix)[i][j] = lambda;
            } else if (j == i-1) {
                (*theRateMatrix)[i][j] = delta;
            } else if (j == ((2*i)+1)) {
                (*theRateMatrix)[i][j] = rho;
            } else {
                (*theRateMatrix)[i][j] = 0.0;
            }
        }
    }
    
    // set the diagonal values
    setDiagonal();
    
    // do I need to rescale the rates??
    //rescaleToAverageRate( 1.0 );
}




/** Calculate the transition probabilities */
void RateMatrix_Chromosomes::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
   // std::cout << "In calculateTransitionProbabilities: "<< t <<std::endl;
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
	
	// Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003)
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
    return;
}

void RateMatrix_Chromosomes::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary. 
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P[i][j] = (*theRateMatrix)[i][j] * tOver2s; 
        }
    }
    //Add the identity matrix:
     for ( size_t i = 0; i < matrixSize; i++ ) {
         P[i][i] += 1;
     }
     //Now we can do the multiplications
     TransitionProbabilityMatrix P2 (matrixSize);
     squareMatrix (P, P2); //P2 at power 2
     squareMatrix (P2, P); //P at power 4
     squareMatrix (P, P2); //P2 at power 8
     squareMatrix (P2, P); //P at power 16
     squareMatrix (P, P2); //P2 at power 32
     squareMatrix (P2, P); //P at power 64
     squareMatrix (P, P2); //P2 at power 128
     squareMatrix (P2, P); //P at power 256
     squareMatrix (P, P2); //P2 at power 512
     squareMatrix (P2, P); //P at power 1024

     return;
}

inline void RateMatrix_Chromosomes::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < matrixSize; i++ ) {
        for ( size_t j = 0; j < matrixSize; j++ ) {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < matrixSize; k++ ) {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
                }
            }
        }
    }



RateMatrix_Chromosomes* RateMatrix_Chromosomes::clone( void ) const {
    return new RateMatrix_Chromosomes( *this );
}


const std::vector<double>& RateMatrix_Chromosomes::getStationaryFrequencies( void ) const {
    
    return stationaryFreqs;
}


void RateMatrix_Chromosomes::updateMatrix( void ) {
    
    if ( needsUpdate )
    {
        buildRateMatrix();
        // clean flags
        needsUpdate = false;
    }
}


