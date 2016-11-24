//
//  RateMatrix_FreeK.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/4/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_FreeK.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_FreeK::RateMatrix_FreeK(size_t n) : GeneralRateMatrix( n ),
    rescale(true)
{
    
    theEigenSystem       = new EigenSystem(the_rate_matrix);
    c_ijk.resize(num_states * num_states * num_states);
    cc_ijk.resize(num_states * num_states * num_states);
    
    update();
}


RateMatrix_FreeK::RateMatrix_FreeK(size_t n, bool r) : GeneralRateMatrix( n ),
    rescale(r)
{
    
    theEigenSystem       = new EigenSystem(the_rate_matrix);
    c_ijk.resize(num_states * num_states * num_states);
    cc_ijk.resize(num_states * num_states * num_states);
    
    update();
}


/** Copy constructor */
RateMatrix_FreeK::RateMatrix_FreeK(const RateMatrix_FreeK& m) : GeneralRateMatrix( m )
{
    
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(the_rate_matrix);
}


/** Destructor */
RateMatrix_FreeK::~RateMatrix_FreeK(void)
{
    
    delete theEigenSystem;
}


RateMatrix_FreeK& RateMatrix_FreeK::operator=(const RateMatrix_FreeK &r)
{
    
    if (this != &r) 
    {
        GeneralRateMatrix::operator=( r );
       
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(the_rate_matrix);

    }
    
    return *this;
}


void RateMatrix_FreeK::fillRateMatrix( void )
{
    
    MatrixReal& m = *the_rate_matrix;
    
    // fill the rate matrix
    for (size_t i=0, k=0; i<num_states; i++)
    {
        double sum = 0.0;
        
        // off-diagonal
        for (size_t j=0; j<num_states; j++)
        {
            if (i==j)
            {
                continue;
            }
            double r = transition_rates[k];
            sum += r;
            m[i][j] = r;
            k++;
        }
        
        // diagonal
        m[i][i] = -sum;
    }

    // set flags
    needs_update = true;
}

/** Do precalculations on eigenvectors */
void RateMatrix_FreeK::calculateCijk(void)
{
    
    if ( theEigenSystem->isComplex() == false )
    {
        // real case
        const MatrixReal& ev  = theEigenSystem->getEigenvectors();
        const MatrixReal& iev = theEigenSystem->getInverseEigenvectors();
        double* pc = &c_ijk[0];
        for (size_t i=0; i<num_states; i++)
        {
            
            for (size_t j=0; j<num_states; j++)
            {
                
                for (size_t k=0; k<num_states; k++)
                {
                    *(pc++) = ev[i][k] * iev[k][j];
                }
                
            }
            
        }
        
    }
    else
    {
        // complex case
        const MatrixComplex& cev  = theEigenSystem->getComplexEigenvectors();
        const MatrixComplex& ciev = theEigenSystem->getComplexInverseEigenvectors();
        std::complex<double>* pc = &cc_ijk[0];
        for (size_t i=0; i<num_states; i++)
        {
            
            for (size_t j=0; j<num_states; j++)
            {
                
                for (size_t k=0; k<num_states; k++)
                {
                    *(pc++) = cev[i][k] * ciev[k][j];
                }
                
            }
            
        }
        
    }
    
}


/** Calculate the transition probabilities */
void RateMatrix_FreeK::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const
{
    // The eigensystem code was returning NaN likelihood values when transition rates
    // were close to 0.0, so now we use repeated squaring. Unfortunately repeated
    // squaring is slower.
    // Will Freyman 11/16/16
    
//    double t = rate * (startAge - endAge);
//	if ( theEigenSystem->isComplex() == false )
//    {
//		tiProbsEigens(t, P);
//    }
//	else
//    {
//		tiProbsComplexEigens(t, P);
//    }
    
    
    // Repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    // Mayrose et al. 2010 also used this method for chromosome evolution (named the squaring and scaling
    // method in Moler and Van Loan 2003)
    double t = rate * (startAge - endAge);
    computeExponentialMatrixByRepeatedSquaring(t, P);
    
}

void RateMatrix_FreeK::computeExponentialMatrixByRepeatedSquaring(double t,  TransitionProbabilityMatrix& P ) const {
    
    
    // We use repeated squaring to quickly obtain exponentials, as in Poujol and Lartillot, Bioinformatics 2014.
    // Ideally one should dynamically decide how many squarings are necessary.
    // For the moment, we arbitrarily do 10 such squarings, as it seems to perform well in practice (N. Lartillot, personal communication).
    // first, multiply the matrix by the right scalar
    // 2^10 = 1024
    
    size_t matrixSize = num_states;
    
    double tOver2s = t/(1024);
    for ( size_t i = 0; i < matrixSize; i++ )
    {
        for ( size_t j = 0; j < matrixSize; j++ )
        {
            P[i][j] = (*the_rate_matrix)[i][j] * tOver2s;
        }
    }
    
    // Add the identity matrix:
    for ( size_t i = 0; i < matrixSize; i++ )
    {
        P[i][i] += 1;
    }
    
    // Now we can do the multiplications
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

inline void RateMatrix_FreeK::squareMatrix( TransitionProbabilityMatrix& P,  TransitionProbabilityMatrix& P2) const {
    
    //Could probably use boost::ublas here, for the moment we do it ourselves.
    size_t matrixSize = num_states;
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



RateMatrix_FreeK* RateMatrix_FreeK::clone( void ) const
{
    return new RateMatrix_FreeK( *this );
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_FreeK::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const
{
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValue = theEigenSystem->getRealEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<double> eigValExp(num_states);
	for (size_t s=0; s<num_states; s++)
    {
		eigValExp[s] = exp(eigenValue[s] * t);
    }
    
    // calculate the transition probabilities
	const double* ptr = &c_ijk[0];
    double*         p = P.theMatrix;
	for (size_t i=0; i<num_states; i++)
    {
		for (size_t j=0; j<num_states; j++, ++p)
        {
			double sum = 0.0;
			for(size_t s=0; s<num_states; s++)
            {
				sum += (*ptr++) * eigValExp[s];
            }
            
            //			P[i][j] = (sum < 0.0) ? 0.0 : sum;
			(*p) = (sum < 0.0) ? 0.0 : sum;
        }
        
    }
    
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix_FreeK::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const
{
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValueReal = theEigenSystem->getRealEigenvalues();
    const std::vector<double>& eigenValueComp = theEigenSystem->getImagEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<std::complex<double> > ceigValExp(num_states);
	for (size_t s=0; s<num_states; s++)
    {
        std::complex<double> ev = std::complex<double>(eigenValueReal[s], eigenValueComp[s]);
		ceigValExp[s] = exp(ev * t);
    }
    
    // calculate the transition probabilities
	const std::complex<double>* ptr = &cc_ijk[0];
	for (size_t i=0; i<num_states; i++)
    {
		for (size_t j=0; j<num_states; j++)
        {
			std::complex<double> sum = std::complex<double>(0.0, 0.0);
			for(size_t s=0; s<num_states; s++)
            {
				sum += (*ptr++) * ceigValExp[s];
            }
            
            P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
        }
        
    }
    
}


/** Update the eigen system */
void RateMatrix_FreeK::updateEigenSystem(void)
{
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_FreeK::update( void )
{
    
    if ( needs_update )
    {
        // assign all rate matrix elements
        fillRateMatrix();

        // rescale
        if ( rescale == true )
        {
            rescaleToAverageRate( 1.0 );
        }

        // now update the eigensystem
//        updateEigenSystem();
        
        // clean flags
        needs_update = false;
    }
    
}

