/**
 * @file
 * This file contains the implementation of RateMatrix_GTR, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix_GTR
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
 * $Id: RateMatrix_GTR.cpp 1921 2012-12-11 13:46:24Z hoehna $
 */

#include "EigenSystem.h"
#include "MatrixComplex.h"
#include "MatrixReal.h"
#include "RateMatrix_GTR.h"
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
RateMatrix_GTR::RateMatrix_GTR(size_t n) : RateMatrix( n ){
    
    eigensDirty          = true;
    reversibilityChecked = false;
    timeReversible       = false;
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates * numStates * numStates);
    cc_ijk.resize(numStates * numStates * numStates);
    
}


/** Copy constructor */
RateMatrix_GTR::RateMatrix_GTR(const RateMatrix_GTR& m) : RateMatrix( m ) {
    
    eigensDirty          = m.eigensDirty;
    reversibilityChecked = m.reversibilityChecked;
    timeReversible       = m.timeReversible;
    theEigenSystem       = new EigenSystem( *m.theEigenSystem );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix_GTR::~RateMatrix_GTR(void) {
    
    delete theEigenSystem;
}


RateMatrix_GTR& RateMatrix_GTR::operator=(const RateMatrix_GTR &r) {
    
    if (this != &r) {
        RateMatrix::operator=( r );
        
        delete theEigenSystem;
        
        eigensDirty          = r.eigensDirty;
        reversibilityChecked = r.reversibilityChecked;
        timeReversible       = r.timeReversible;
        theEigenSystem       = new EigenSystem( *r.theEigenSystem );
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
    }
    
    return *this;
}



bool RateMatrix_GTR::areEigensDirty( void ) const {
    return eigensDirty;
}



/** Do precalculations on eigenvectors */
void RateMatrix_GTR::calculateCijk(void) {
    
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


/** This function calculates the stationary frequencies of the rate matrix. The
 rate matrix, Q, is the infinitesimal generator of the Markov chain. It is an
 n X n matrix whose off-diagonal elements are q_ij >= 0 and whose diagonal elements
 are specified such that each row sums to zero. The rate matrix is finite (has
 a fixed number of states) and we assume that the input matrix is irreducible, as
 is the usual case for substitution models. Because Q is irreducible and finite,
 it has a stationary distribution, pi, which is a row vector of n probabilities.
 The stationary probabilities can be calculated by solving the homogeneous system
 of equations, pi*Q = 0, where 0 is a vector of zeros.
 
 We do the following to calculate the stationary frequencies. 
 
 1. We perform an LU decomposition of the transpose of the matrix Q.
 
 Q' = LU
 
 2. Now we set Ux = z (x will eventually hold the stationary probabilities). 
 Because L is nonsingular, we have z = 0. We proceed to back substitute on
 Ux = z = 0. When u_nn = 0, we can put in any solution for x. Here, we put
 in x_n = 1. We then solve the other values of x through back substitution.
 
 3. The solution obtained in 2 is not a probability vector. We normalize the
 vector such that the sum of the elements is 1.
 
 Note that the only time we need to use this function is when we don't
 know the stationary frequencies of the rate matrix beforehand. For most
 substitution models used in molecular evolution, the stationary frequencies
 are built into the rate matrix itself. These models are time-reversible.
 This function is useful for the non-reversible models.
 
 For more information on the fascinating topic of calculating the stationary
 probabilities of a rate matrix, see:
 
 Stewart, W. J. 1999. Numerical methods for computing stationary distributions of 
 finite irreducible Markov chains. In "Advances in Computational
 Probability", W. Grassmann, ed. Kluwer Academic Publishers. */
void RateMatrix_GTR::calculateStationaryFrequencies(void) {
    
	// transpose the rate matrix and put into QT
    MatrixReal QT(numStates, numStates);
    for (size_t i=0; i<numStates; i++)
        for (size_t j=0; j<numStates; j++)
            QT[i][j] = (*theRateMatrix)[j][i];
    
	// compute the LU decomposition of the transposed rate matrix
    MatrixReal L(numStates, numStates);
    MatrixReal U(numStates, numStates);
	RbMath::computeLandU(QT, L, U);
	
	// back substitute into z = 0 to find un-normalized stationary frequencies, starting with x_n = 1.0
    std::vector<double> pi(numStates, 0.0);
	pi[numStates-1] = 1.0;
	for (int i= int(numStates-2); i >= 0; i--)
    {
		double dotProduct = 0.0;
		for (size_t j=i+1; j<numStates; j++)
			dotProduct += U[i][j] * pi[j];
		pi[i] = (0.0 - dotProduct) / U[i][i];
    }
    
	// normalize the solution vector
	double sum = 0.0;
	for (size_t i=0; i<numStates; i++)
		sum += pi[i];
	for (size_t i=0; i<numStates; i++)
		pi[i] /= sum;
    
    // set the stationary frequencies
    theStationaryFreqs = pi;
}


/** Calculate the transition probabilities */
void RateMatrix_GTR::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {
    
	if ( theEigenSystem->isComplex() == false )
		tiProbsEigens(t, P);
	else
		tiProbsComplexEigens(t, P);
}


RateMatrix_GTR* RateMatrix_GTR::clone( void ) const {
    return new RateMatrix_GTR( *this );
}


/** This function checks that the rate matrix is time reversible. It takes as
 input the rate matrix, a, and the stationary frequencies of the process, f. 
 It checks that f[i] * q[i][j] = f[j] * q[j][i] for all i != j. It does this
 by accumulating the difference | f[i] * q[i][j] - f[j] * q[j][i] | for all
 off-diagonal comparisons. If this difference is less than tolerance,
 it reports that the rate matrix is time-reversible. If the flag isRev
 is set to true, then we do not need to check because then we have determined
 previously that the rate matrix is reversible. */ 
bool RateMatrix_GTR::checkTimeReversibity(double tolerance) {
	
	double diff = 0.0;
	for (size_t i=0; i<numStates; i++)
		for (size_t j=i+1; j<numStates; j++)
			diff += fabs( theStationaryFreqs[i] * (*theRateMatrix)[i][j] - theStationaryFreqs[j] * (*theRateMatrix)[j][i] );
    reversibilityChecked = true;
	if (diff < tolerance)
        return true;
	return false;
}


/** Return whether or not the rate matrix is time reversible */
bool RateMatrix_GTR::isTimeReversible(void) {
    
    if (reversibilityChecked == false)
        return checkTimeReversibity(0.000001);
    return timeReversible;
}


/** Directly set whether or not the rate matrix is time reversible */
void RateMatrix_GTR::setTimeReversible(bool tf) {
    
    reversibilityChecked = true;
    timeReversible = tf;
}



/** Calculate the transition probabilities for the real case */
void RateMatrix_GTR::tiProbsEigens(double t, TransitionProbabilityMatrix& P) const {
    
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
void RateMatrix_GTR::tiProbsComplexEigens(double t, TransitionProbabilityMatrix& P) const {
    
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


void RateMatrix_GTR::update( void ) {
    eigensDirty = true;
}


/** Update the eigen system */
void RateMatrix_GTR::updateEigenSystem(void) {
    
    if ( areEigensDirty() ) {
        theEigenSystem->update();
        calculateCijk();
        eigensDirty = false;
    }
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateMatrix_GTR& x) {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << "[ ";
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the RbMatrix with each column of equal width and each column centered on the decimal
    for (size_t i=0; i < x.size(); i++) {
        if (i == 0)
            o << "[ ";
        else 
            o << "  ";
        
        for (size_t j = 0; j < x.size(); ++j) 
        {
            if (j != 0)
                o << ", ";
            o << x[i][j];
        }
        o <<  " ]";
        
        if (i == x.size()-1)
            o << " ]";
        else 
            o << " ,\n";
        
    }
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    return o;
}

