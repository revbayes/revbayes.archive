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
RateMatrix_BDR::RateMatrix_BDR(size_t n) : AbstractRateMatrix( 2 + (0.5 * n * (n + 1) + n) ),
    matrixSize( 2 + (0.5 * n * (n + 1) + n) ),
    maxGenes( n )
{
    
    setAlpha(1.0);
    setBeta(1.0);
    setLambdaA(1.0);
    setMuA(1.0);
    setLambdaI(1.0);
    setMuI(1.0);
    setLambdaAI(1.0);
    setLambdaIA(1.0);
    setDenovoA(1.0);
    setDenovoI(1.0);

    // we need to make sure that the rate matrix is initialized to 0.0
    for (size_t i = 0; i < matrixSize; ++i)
    {
        for (size_t j = 0; j < matrixSize; ++j)
        {
            (*the_rate_matrix)[i][j] = 0.0;
        }
    }
    
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

    // set up de novo elements of rate matrix.
				
    // birth to inactive
    (*the_rate_matrix)[0][1] = denovo_i;
				
    // birth to active
    (*the_rate_matrix)[0][2] = denovo_a;
    
    // set up the rest of the matrix
    int i;
    
    for (size_t fam_size = 1; fam_size <= maxGenes; ++fam_size)
    {
        for (size_t num_active = 0; num_active <= fam_size; ++num_active)
        {
            // given we know fam_size and num_active (the tuple)
            // what tuples do we touch, an what are the parameters for
            // those transitions
            
            i = 0.5 * fam_size * (fam_size + 1) + num_active;
            (*the_rate_matrix)[i][matrixSize-1] = 0.0; // just to be safe
            
            // birth to inactive
            if ( i + fam_size + 2 < matrixSize )
            {
                (*the_rate_matrix)[i][i+fam_size+1] = num_active * lambda_ai + (fam_size - num_active) * lambda_i;
            }
            else
            {
                (*the_rate_matrix)[i][matrixSize-1] = num_active * lambda_ai + (fam_size - num_active) * lambda_i;
            }
            
            // birth to active
            if ( i + fam_size + 3 < matrixSize )
            {
                (*the_rate_matrix)[i][i+fam_size+2] = (fam_size - num_active) * lambda_ia + num_active * lambda_a;
            }
            else
            {
                (*the_rate_matrix)[i][matrixSize-1] += (fam_size - num_active) * lambda_ia + num_active * lambda_a;
            }
            
            if(num_active > 0){
                // death of active
                (*the_rate_matrix)[i][i-fam_size-1] = mu_a * num_active;
                
                // regulatory inactivation
                (*the_rate_matrix)[i][i-1] = beta * num_active;
            }
            
            if(fam_size > num_active){
                // death of inactive
                (*the_rate_matrix)[i][i-fam_size] = mu_i * (fam_size - num_active);
                
                // regulatory activation
                (*the_rate_matrix)[i][i+1] = alpha * (fam_size - num_active);
            }
        }
    }
    
    // set the diagonal values
    setDiagonal();
    
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
    std::vector<double> stationary_freqs(matrixSize, 1 / matrixSize);
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

void RateMatrix_BDR::setDenovoI(double d_i)
{
    denovo_a = d_i;
    
    // set flags
    needs_update = true;
}

void RateMatrix_BDR::setDenovoA(double d_a)
{
    denovo_i = d_a;
    
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


