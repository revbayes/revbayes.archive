/**
 * @file
 * This file contains the implementation of RateMatrix_Ordered, which is a
 * class that holds a rate matrix for the chromosome number evolution model.
 *
 * @brief Implementation of RateMatrix_Ordered
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */

#include "RateMatrix_Ordered.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Ordered::RateMatrix_Ordered(size_t n) : AbstractRateMatrix( n ),
    matrix_size( n )
{
    setLambda(1.0);
    setMu(1.0);
    
    update();
}


/** Destructor */
RateMatrix_Ordered::~RateMatrix_Ordered(void)
{
    
}

double RateMatrix_Ordered::averageRate(void) const
{
    return 1.0;
}


void RateMatrix_Ordered::buildRateMatrix(void)
{
    
    for (size_t i=0; i< matrix_size; i++)
    {
        for (size_t j=0; j< matrix_size; j++)
        {
            (*the_rate_matrix)[i][j] = 0.0;
            if (j != 0 && i != 0) {
                if (j == i+1)
                {
                    (*the_rate_matrix)[i][j] = lambda;
                } else if (j == i-1)
                {
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
void RateMatrix_Ordered::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    
    //Now the instantaneous rate matrix has been filled up entirely.
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    
    // Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003)
    double t = rate * (startAge - endAge);
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
}

void RateMatrix_Ordered::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const
{
    //We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    //Ideally one should dynamically decide how many squarings are necessary.
    //For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    //first, multiply the matrix by the right scalar
    //2^10 = 1024
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrix_size; i++ )
    {
        for ( size_t j = 0; j < matrix_size; j++ )
        {
            P[i][j] = (*the_rate_matrix)[i][j] * tOver2s;
        }
    }
    
    //Add the identity matrix:
    for ( size_t i = 0; i < matrix_size; i++ )
    {
        P[i][i] += 1;
    }
    
    //Now we can do the multiplications
    TransitionProbabilityMatrix P2 (matrix_size);
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

inline void RateMatrix_Ordered::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const
{
    
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < matrix_size; i++ )
    {
        for ( size_t j = 0; j < matrix_size; j++ )
        {
            P2.getElement ( i, j ) = 0;
            for ( size_t k = 0; k < matrix_size; k++ )
            {
                P2.getElement ( i, j ) += P.getElement ( i, k ) * P.getElement ( k, j );
            }
        }
    }
    
}



RateMatrix_Ordered* RateMatrix_Ordered::clone( void ) const
{
    return new RateMatrix_Ordered( *this );
}


std::vector<double> RateMatrix_Ordered::getStationaryFrequencies( void ) const
{
    
    return stationary_freqs;
}

void RateMatrix_Ordered::setLambda( double l )
{
    
    lambda = l;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_Ordered::setMu( double m )
{
    
    mu = m;
    
    // set flags
    needs_update = true;
    
}




void RateMatrix_Ordered::update( void )
{
    
    if ( needs_update )
    {
        buildRateMatrix();
        // clean flags
        needs_update = false;
    }
    
}


