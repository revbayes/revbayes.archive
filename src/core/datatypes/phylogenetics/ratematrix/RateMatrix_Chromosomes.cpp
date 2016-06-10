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
RateMatrix_Chromosomes::RateMatrix_Chromosomes(size_t n) : AbstractRateMatrix( n+1 ),
    matrixSize( n+1 )
{
    setLambda(1.0);
    setRho(1.0);
    setDelta(1.0);
    setMu(1.0);
    setLambda_l(1.0);
    setDelta_l(1.0);
    
    update();
}


/** Destructor */
RateMatrix_Chromosomes::~RateMatrix_Chromosomes(void) {
    
}

double RateMatrix_Chromosomes::averageRate(void) const {
    return 1.0;
}


void RateMatrix_Chromosomes::buildRateMatrix(void) 
{
    
    for (size_t i=0; i< matrixSize; i++) {
        for (size_t j=0; j< matrixSize; j++) {
			(*the_rate_matrix)[i][j] = 0.0;
			if (j != 0 && i != 0) {
				if (j == i+1) {
					(*the_rate_matrix)[i][j] = lambda * exp( lambda_l * (i-1) );
				} else if (j == i-1) {
					(*the_rate_matrix)[i][j] = delta * exp( delta_l * (i-1) );
				} else if (j == (2*i)) {
					(*the_rate_matrix)[i][j] = rho;
                } else if ( (i % 2 == 0) && (j == (size_t)(1.5*i)) ) {
                    (*the_rate_matrix)[i][j] = mu;
                } else if ( (i % 2 != 0) && ( (j == (size_t)(1.5*i - 0.5)) || (j == (size_t)(1.5*i + 0.5) ) ) ) {
                    (*the_rate_matrix)[i][j] = mu;
                }
			}
        }
    }	
    // set the diagonal values
    setDiagonal();
    
    // rescale rates
    //rescaleToAverageRate( 1.0 );
}




/** Calculate the transition probabilities */
void RateMatrix_Chromosomes::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
	
	// Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003)
    double t = rate * (startAge - endAge);
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
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
            P[i][j] = (*the_rate_matrix)[i][j] * tOver2s; 
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
    for ( size_t i = 0; i < matrixSize; i++ )
    {
        for ( size_t j = 0; j < matrixSize; j++ )
        {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < matrixSize; k++ )
            {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
            }
        }
    }
}



RateMatrix_Chromosomes* RateMatrix_Chromosomes::clone( void ) const
{
    return new RateMatrix_Chromosomes( *this );
}


std::vector<double> RateMatrix_Chromosomes::getStationaryFrequencies( void ) const
{
    
    return stationary_freqs;
}

void RateMatrix_Chromosomes::setLambda( double l )
{
    
    lambda = l;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Chromosomes::setRho( double r )
{
    
    rho = r;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Chromosomes::setDelta( double d ) {
    
    delta = d;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Chromosomes::setMu( double m ) {
    
    mu = m;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Chromosomes::setLambda_l( double l ) {
    
    lambda_l = l;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Chromosomes::setDelta_l( double d ) {
    
    delta_l = d;
    
    // set flags
    needs_update = true;
    
}


void RateMatrix_Chromosomes::update( void ) {
    
    if ( needs_update )
    {
        buildRateMatrix();
        // clean flags
        needs_update = false;
    }
}


