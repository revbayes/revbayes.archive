/**
 * @file
 * This file contains the implementation of RateMatrix_BDR, which is a
 * class that holds a rate matrix for the chromosome number evolution model. 
 *
 * @brief Implementation of RateMatrix_BDR
 *
 * (c) copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */

#include "RateMatrix_BDR.h"
#include "MatrixReal.h"
#include "RbException.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_BDR::RateMatrix_BDR(size_t n) : AbstractRateMatrix( n+1 ),
    matrixSize( n+1 )
{
    
    setAlpha(1.0);
    setBeta(1.0);
    setLambdaA(1.0);
    setMuA(1.0);
    setLambdaI(1.0);
    setMuI(1.0);
    setLambdaAI(1.0);
    setLambdaIA(1.0);
    
    update();
    
}


/** Destructor */
RateMatrix_BDR::~RateMatrix_BDR(void) {
    
}

double RateMatrix_BDR::averageRate(void) const {
    return 1.0;
}


void RateMatrix_BDR::buildRateMatrix(void) 
{
    
//    for (size_t i=0; i< matrixSize; i++)
//    {
//        for (size_t j=0; j< matrixSize; j++)
//        {
//			(*the_rate_matrix)[i][j] = 0.0;
//			if (j != 0 && i != 0)
//            {
//				if (j == i+1)
//                {
//					(*the_rate_matrix)[i][j] += lambda * exp( lambda_l * (i-1) );
//				}
//                if (j == i-1)
//                {
//					(*the_rate_matrix)[i][j] += delta * exp( delta_l * (i-1) );
//				}
//                if (j == (2*i))
//                {
//					(*the_rate_matrix)[i][j] += rho;
//                }
//                if ( (i % 2 == 0) && (j == (size_t)(1.5*i)) )
//                {
//                    (*the_rate_matrix)[i][j] += mu;
//                }
//                if ( (i % 2 != 0) && ( (j == (size_t)(1.5*i - 0.5)) || (j == (size_t)(1.5*i + 0.5) ) ) )
//                {
//                    (*the_rate_matrix)[i][j] += mu;
//                }
//			}
//        }
//    }	
//    // set the diagonal values
//    setDiagonal();
//    
//    // rescale rates
//    //rescaleToAverageRate( 1.0 ); # TODO: We actually DO want to rescale the rate matrix... implement this function
    
}




/** Calculate the transition probabilities */
void RateMatrix_BDR::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    // We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
	// Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling method in Moler and Van Loan 2003).
    double t = rate * (startAge - endAge);
    exponentiateMatrixByScalingAndSquaring(t, P);
    
}



void RateMatrix_BDR::exponentiateMatrixByScalingAndSquaring(double t,  TransitionProbabilityMatrix& p) const {
    
    // Here we use the scaling and squaring method with a 4th order Taylor approximant as described in:
    //
    // Moler, C., & Van Loan, C. 2003. Nineteen dubious ways to compute the exponential of a
    // matrix, twenty-five years later. SIAM review, 45(1), 3-49.
    //
    // I tested this implementation against the Eigen C++ package and a scaling parameter s = 6 had similar time
    // efficiency and returned the same results with about 10^-9 accuracy. The scaling parameter could be
    // increased for better accuracy.
    // -- Will Freyman 11/27/16
    size_t s = 6;
    
    // first scale the matrix
    double scale = t / pow(2, s);
    for ( size_t i = 0; i < num_states; i++ )
    {
        for ( size_t j = 0; j < num_states; j++ )
        {
            p[i][j] = (*the_rate_matrix)[i][j] * scale;
        }
    }
    
    // compute the 4th order Taylor approximant
    
    // calculate the scaled matrix raised to powers 2, 3 and 4
    TransitionProbabilityMatrix p_2(num_states);
    multiplyMatrices(p, p, p_2);
    
    TransitionProbabilityMatrix p_3(num_states);
    multiplyMatrices(p, p_2, p_3);
    
    TransitionProbabilityMatrix p_4(num_states);
    multiplyMatrices(p, p_3, p_4);
    
    // add k=0 (the identity matrix) and k=1 terms
    for ( size_t i = 0; i < num_states; i++ )
    {
        p[i][i] += 1;
    }
    
    // add the k=2, k=3, k=4 terms of the Taylor series
    for ( size_t i = 0; i < num_states; i++ )
    {
        for ( size_t j = 0; j < num_states; j++ )
        {
            p[i][j] += ( ( p_2[i][j] / 2 ) + ( p_3[i][j] / 6 ) + ( p_4[i][j] / 24 ) );
        }
    }
    
    // now perform the repeated squaring
    for (size_t i = 0; i < s; i++)
    {
        TransitionProbabilityMatrix r(num_states);
        multiplyMatrices(p, p, r);
        p = r;
    }
}



inline void RateMatrix_BDR::multiplyMatrices(TransitionProbabilityMatrix& p,  TransitionProbabilityMatrix& q,  TransitionProbabilityMatrix& r) const {
    
    // could probably use boost::ublas here, for the moment we do it ourselves.
    for ( size_t i = 0; i < num_states; i++ )
    {
        for ( size_t j = 0; j < num_states; j++ )
        {
            r[i][j] = 0;
            for ( size_t k = 0; k < num_states; k++ )
            {
                r[i][j] += p[i][k] * q[k][j];
            }
        }
    }
}



RateMatrix_BDR* RateMatrix_BDR::clone( void ) const
{
    return new RateMatrix_BDR( *this );
}


std::vector<double> RateMatrix_BDR::getStationaryFrequencies( void ) const
{
    
    return stationary_freqs;
}


void RateMatrix_BDR::setAlpha(double a)
{
    alpha = a;
    
    // set flags
    needs_update = true;
}

void RateMatrix_BDR::setBeta(double b)
{
    beta = b;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_BDR::setLambdaA(double l_a)
{
    lambda_a = l_a;
    
    // set flags
    needs_update = true;
  
}

void RateMatrix_BDR::setMuA(double m_a)
{
    mu_a = m_a;
    
    // set flags
    needs_update = true;
   
}

void RateMatrix_BDR::setLambdaI(double l_i)
{
    lambda_i = l_i;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_BDR::setMuI(double m_i)
{
    mu_i = m_i;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_BDR::setLambdaAI(double l_ai)
{
    lambda_ai = l_ai;
    
    // set flags
    needs_update = true;
    
}

void RateMatrix_BDR::setLambdaIA(double l_ia)
{
    lambda_ia = l_ia;
    
    // set flags
    needs_update = true;
}


void RateMatrix_BDR::update( void ) {
    
    if ( needs_update )
    {
        buildRateMatrix();
        // clean flags
        needs_update = false;
    }
}


