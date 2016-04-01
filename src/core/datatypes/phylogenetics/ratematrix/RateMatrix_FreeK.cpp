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
RateMatrix_FreeK::RateMatrix_FreeK(size_t n) : GeneralRateMatrix( n )
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

double RateMatrix_FreeK::averageRate(void) const
{
    return 1.0;
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
    double t = rate * (startAge - endAge);
	if ( theEigenSystem->isComplex() == false )
    {
		tiProbsEigens(t, P);
    }
	else
    {
		tiProbsComplexEigens(t, P);
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
        rescaleToAverageRate( 1.0 );

        // now update the eigensystem
        updateEigenSystem();
        
        // clean flags
        needs_update = false;
    }
    
}

