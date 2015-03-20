#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_Empirical.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateMatrix_Empirical::RateMatrix_Empirical(size_t n) : TimeReversibleRateMatrix( n ){
    
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
    update();
}


/** Copy constructor */
RateMatrix_Empirical::RateMatrix_Empirical(const RateMatrix_Empirical& m) : TimeReversibleRateMatrix( m ) {
    
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_Empirical::~RateMatrix_Empirical(void) {
    
    delete theEigenSystem;
}


RateMatrix_Empirical& RateMatrix_Empirical::operator=(const RateMatrix_Empirical &r) {
    
    if (this != &r) {
        TimeReversibleRateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
    }
    
    return *this;
}



/** Do precalculations on eigenvectors */
void RateMatrix_Empirical::calculateCijk(void) {
    
    if ( theEigenSystem->isComplex() == false )
    {
        // real case
        const MatrixReal& ev  = theEigenSystem->getEigenvectors();
        const MatrixReal& iev = theEigenSystem->getInverseEigenvectors();
        double* pc = &c_ijk[0];
        for (size_t i=0; i<numStates; i++)
            for (size_t j=0; j<numStates; j++)
                for (size_t k=0; k<numStates; k++)
                    *(pc++) = ev[i][k] * iev[k][j];   
    }
    else
    {
        // complex case
        const MatrixComplex& cev  = theEigenSystem->getComplexEigenvectors();
        const MatrixComplex& ciev = theEigenSystem->getComplexInverseEigenvectors();
        std::complex<double>* pc = &cc_ijk[0];
        for (size_t i=0; i<numStates; i++)
            for (size_t j=0; j<numStates; j++)
                for (size_t k=0; k<numStates; k++)
                    *(pc++) = cev[i][k] * ciev[k][j];
    }
}


/** Calculate the transition probabilities */
void RateMatrix_Empirical::calculateTransitionProbabilities(double startAge, double endAge, double rate, TransitionProbabilityMatrix& P) const {
    
    double t = rate * (startAge - endAge);
	if ( theEigenSystem->isComplex() == false )
		tiProbsEigens(t, P);
	else
		tiProbsComplexEigens(t, P);
}


RateMatrix_Empirical* RateMatrix_Empirical::clone( void ) const {
    return new RateMatrix_Empirical( *this );
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_Empirical::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const {
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValue = theEigenSystem->getRealEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<double> eigValExp(numStates);
	for (size_t s=0; s<numStates; s++)
		eigValExp[s] = exp(eigenValue[s] * t);
    
    // calculate the transition probabilities
	const double* ptr = &c_ijk[0];
	for (size_t i=0; i<numStates; i++) 
    {
		for (size_t j=0; j<numStates; j++) 
        {
			double sum = 0.0;
			for(size_t s=0; s<numStates; s++)
				sum += (*ptr++) * eigValExp[s];
			P[i][j] = (sum < 0.0) ? 0.0 : sum;
        }
    }
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix_Empirical::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const {
    
    // get a reference to the eigenvalues
    const std::vector<double>& eigenValueReal = theEigenSystem->getRealEigenvalues();
    const std::vector<double>& eigenValueComp = theEigenSystem->getImagEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<std::complex<double> > ceigValExp(numStates);
	for (size_t s=0; s<numStates; s++)
    {
        std::complex<double> ev = std::complex<double>(eigenValueReal[s], eigenValueComp[s]);
		ceigValExp[s] = exp(ev * t);
    }
    
    // calculate the transition probabilities
	const std::complex<double>* ptr = &cc_ijk[0];
	for (size_t i=0; i<numStates; i++) 
    {
		for (size_t j=0; j<numStates; j++) 
        {
			std::complex<double> sum = std::complex<double>(0.0, 0.0);
			for(size_t s=0; s<numStates; s++)
				sum += (*ptr++) * ceigValExp[s];
			P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
        }
    }
}


/** Update the eigen system */
void RateMatrix_Empirical::updateEigenSystem(void) {
    
    theEigenSystem->update();
    calculateCijk();
    
}


void RateMatrix_Empirical::update( void ) {
    
}


