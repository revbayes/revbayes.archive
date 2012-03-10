/**
 * @file
 * This file contains the implementation of RateMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "EigenSystem.h"
#include "RateMatrix.h"
#include "Matrix.h"
#include "MemberFunction.h"
#include "Natural.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "RbUtil.h"
#include "RbString.h"
#include "RealPos.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "TransitionProbabilityMatrix.h"
#include "ValueRule.h"
#include "VariableNode.h"
#include "Workspace.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>


/** Constructor passes member rules and method inits to base class */
RateMatrix::RateMatrix(void) : MemberObject(getMemberRules()) {

    numStates            = 2;
    areEigensDirty       = true;
    reversibilityChecked = false;
    isReversible         = false;
    theRateMatrix        = new Matrix<Real>(numStates.getValue(), numStates.getValue(), new Real(0.0) );
    theStationaryFreqs   = new Simplex(numStates.getValue());
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates.getValue() * numStates.getValue() * numStates.getValue());
    cc_ijk.resize(numStates.getValue() * numStates.getValue() * numStates.getValue());
}


/** Construct rate matrix with n states */
RateMatrix::RateMatrix(size_t n) : MemberObject(getMemberRules()) {

    numStates            = n;
    areEigensDirty       = true;
    reversibilityChecked = false;
    isReversible         = false;
    theRateMatrix        = new Matrix<Real>(numStates.getValue(), numStates.getValue(), new Real(0.0) );
    theStationaryFreqs   = new Simplex(numStates.getValue());
    theEigenSystem       = new EigenSystem(theRateMatrix);
    c_ijk.resize(numStates.getValue() * numStates.getValue() * numStates.getValue());
    cc_ijk.resize(numStates.getValue() * numStates.getValue() * numStates.getValue());
}


/** Copy constructor */
RateMatrix::RateMatrix(const RateMatrix& m) {

    numStates            = m.numStates;
    areEigensDirty       = m.areEigensDirty;
    reversibilityChecked = m.reversibilityChecked;
    isReversible         = m.isReversible;
    theRateMatrix        = m.theRateMatrix->clone();
    theStationaryFreqs   = m.theStationaryFreqs->clone();
    theEigenSystem       = m.theEigenSystem->clone();
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    avgRate              = m.avgRate;
    
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix::~RateMatrix(void) {
    
    delete theRateMatrix;
    delete theStationaryFreqs;
    delete theEigenSystem;
}


RateMatrix& RateMatrix::operator=(const RateMatrix &r) {
    
    if (this != &r) {
        MemberObject::operator=(r);
        
        delete theRateMatrix;
        delete theStationaryFreqs;
        delete theEigenSystem;
        
        numStates            = r.numStates;
        areEigensDirty       = r.areEigensDirty;
        reversibilityChecked = r.reversibilityChecked;
        isReversible         = r.isReversible;
        theRateMatrix        = r.theRateMatrix->clone();
        theStationaryFreqs   = r.theStationaryFreqs->clone();
        theEigenSystem       = r.theEigenSystem->clone();
        c_ijk                = r.c_ijk;
        cc_ijk               = r.cc_ijk;
        avgRate              = r.avgRate;
        
        theEigenSystem->setRateMatrixPtr(theRateMatrix);
    }
    
    return *this;
}

/** Index operator (const) */
const RbVector<Real>& RateMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates.getValue() )
        throw RbException( "Index to " + getClassName() + "[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Index operator */
RbVector<Real>& RateMatrix::operator[]( const size_t i ) {

    if ( i >= numStates.getValue() )
        throw RbException( "Index to " + getClassName() + "[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Calculate the average rate for the rate matrix */
double RateMatrix::averageRate(void) const {

    double ave = 0.0;
    for (size_t i=0; i<numStates.getValue(); i++)
        ave += -(*theStationaryFreqs)[i] * (*theRateMatrix)[i][i];
    return ave;
}


/** Do precalculations on eigenvectors */
void RateMatrix::calculateCijk(void) {

    if ( theEigenSystem->isComplex() == false )
        {
        // real case
        const Matrix<Real>& ev  = theEigenSystem->getEigenvectors();
        const Matrix<Real>& iev = theEigenSystem->getInverseEigenvectors();
        double* pc = &c_ijk[0];
        for (size_t i=0; i<numStates.getValue(); i++)
            for (size_t j=0; j<numStates.getValue(); j++)
                for (size_t k=0; k<numStates.getValue(); k++)
                    *(pc++) = ev[i][k] * iev[k][j];   
        }
    else
        {
        // complex case
        const Matrix<Complex>& cev  = theEigenSystem->getComplexEigenvectors();
        const Matrix<Complex>& ciev = theEigenSystem->getComplexInverseEigenvectors();
        std::complex<double>* pc = &cc_ijk[0];
        for (size_t i=0; i<numStates.getValue(); i++)
            for (size_t j=0; j<numStates.getValue(); j++)
                for (size_t k=0; k<numStates.getValue(); k++)
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
void RateMatrix::calculateStationaryFrequencies(void) {

	// transpose the rate matrix and put into QT
    Matrix<Real> QT(numStates.getValue(), numStates.getValue());
    for (size_t i=0; i<numStates.getValue(); i++)
        for (size_t j=0; j<numStates.getValue(); j++)
            QT[i][j] = (*theRateMatrix)[j][i];

	// compute the LU decomposition of the transposed rate matrix
    Matrix<Real> L(numStates.getValue(), numStates.getValue());
    Matrix<Real> U(numStates.getValue(), numStates.getValue());
	RbMath::computeLandU(QT, L, U);
	
	// back substitute into z = 0 to find un-normalized stationary frequencies, starting with x_n = 1.0
    RbVector<Real> pi(numStates.getValue(), new Real(0.0) );
	pi[numStates.getValue()-1] = 1.0;
	for (int i=numStates.getValue()-2; i>=0; i--)
		{
		double dotProduct = 0.0;
		for (size_t j=i+1; j<numStates.getValue(); j++)
			dotProduct += U[i][j] * pi[j];
		pi[i] = (0.0 - dotProduct) / U[i][i];
		}
		
	// normalize the solution vector
	double sum = 0.0;
	for (size_t i=0; i<numStates.getValue(); i++)
		sum += pi[i];
	for (size_t i=0; i<numStates.getValue(); i++)
		pi[i] /= sum;
        
    // set the stationary frequencies
    theStationaryFreqs->setValue(pi);
}


/** Calculate the transition probabilities */
void RateMatrix::calculateTransitionProbabilities(double t, TransitionProbabilityMatrix& P) const {

	if ( theEigenSystem->isComplex() == false )
		tiProbsEigens(t, P);
	else
		tiProbsComplexEigens(t, P);
}


/** This function checks that the rate matrix is time reversible. It takes as
    input the rate matrix, a, and the stationary frequencies of the process, f. 
    It checks that f[i] * q[i][j] = f[j] * q[j][i] for all i != j. It does this
    by accumulating the difference | f[i] * q[i][j] - f[j] * q[j][i] | for all
    off-diagonal comparisons. If this difference is less than tolerance,
    it reports that the rate matrix is time-reversible. If the flag isRev
    is set to true, then we do not need to check because then we have determined
    previously that the rate matrix is reversible. */ 
bool RateMatrix::checkTimeReversibity(double tolerance) {
	
	double diff = 0.0;
	for (size_t i=0; i<numStates.getValue(); i++)
		for (size_t j=i+1; j<numStates.getValue(); j++)
			diff += fabs( (*theStationaryFreqs)[i] * (*theRateMatrix)[i][j] - (*theStationaryFreqs)[j] * (*theRateMatrix)[j][i] );
    reversibilityChecked = true;
	if (diff < tolerance)
        return true;
	return false;
}


/** Clone object */
RateMatrix* RateMatrix::clone(void) const {

    return new RateMatrix(*this);
}


/** Map calls to member methods */
const RbLanguageObject& RateMatrix::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {

    if (name == "nstates") {
        return numStates;
    }
    else if (name == "stationaryfreqs") {
        RbLanguageObject* s = theStationaryFreqs;
        return *s;
    }
    else if (name == "averate") {
        avgRate.setValue( averageRate() );
        return avgRate;
    }        
    else if (name == "reversible") {
        return isReversible;
    }        

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& RateMatrix::getClassName(void) { 
    
    static std::string rbClassName = "Rate matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RateMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RateMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Return whether or not the rate matrix is time reversible */
bool RateMatrix::getIsTimeReversible(void) {

    if (reversibilityChecked == false)
        return checkTimeReversibity(0.000001);
    return isReversible;
}


/** Get member rules */
const MemberRules& RateMatrix::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Get methods */
const MethodTable& RateMatrix::getMethods(void) const {

    static MethodTable methods = MethodTable();
    static ArgumentRules* nstatesArgRules = new ArgumentRules();
    static ArgumentRules* stationaryfreqsArgRules = new ArgumentRules();
    static ArgumentRules* averateArgRules = new ArgumentRules();
    static ArgumentRules* reversibleArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        methods.addFunction("nstates",         new MemberFunction(Natural::getClassTypeSpec(), nstatesArgRules)         );
        methods.addFunction("stationaryfreqs", new MemberFunction(Simplex::getClassTypeSpec(), stationaryfreqsArgRules) );
        methods.addFunction("averate",         new MemberFunction(RealPos::getClassTypeSpec(), averateArgRules)         );
        methods.addFunction("reversible",      new MemberFunction(RbBoolean::getClassTypeSpec(), reversibleArgRules)    );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
        }

    return methods;
}


/** Print value for user */
void RateMatrix::printValue(std::ostream& o) const {

    o << "Rate matrix:" << std::endl;
    theRateMatrix->printValue(o);
    o << std::endl;
    o << "Stationary frequencies: ";
    theStationaryFreqs->printValue(o);
    o << std::endl;
}


/** Rescale the rates such that the average rate is r */
void RateMatrix::rescaleToAverageRate(const double r) {

    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<numStates.getValue(); i++)
        for (size_t j=0; j<numStates.getValue(); j++)
            (*theRateMatrix)[i][j] *= scaleFactor;
}


/** Set the diagonal of the rate matrix such that each row sums to zero */
void RateMatrix::setDiagonal(void) {

    for (size_t i=0; i<numStates.getValue(); i++)
        {
        double sum = 0.0;
        for (size_t j=0; j<numStates.getValue(); j++)
            {
            if (i != j)
                sum += (*theRateMatrix)[i][j];
            }
        (*theRateMatrix)[i][i] = -sum;
        }
}


/** Directly set whether or not the rate matrix is time reversible */
void RateMatrix::setIsTimeReversible(const bool tf) {

    reversibilityChecked = true;
    isReversible = tf;
}


/** Set the stationary frequencies directly. We assume that we know
    what the stationary frequencies are when this function is called. We
    would know the stationary frequencies for most phylogenetic models. */
void RateMatrix::setStationaryFrequencies(const std::vector<double>& f) {

    theStationaryFreqs->setValue(f);
}


/** Calculate the transition probabilities for the real case */
void RateMatrix::tiProbsEigens(const double t, TransitionProbabilityMatrix& P) const {

    // get a reference to the eigenvalues
    const RbVector<Real>& eigenValue = theEigenSystem->getRealEigenvalues();
    
    // precalculate the product of the eigenvalue and the branch length
    std::vector<double> eigValExp(numStates.getValue());
	for (size_t s=0; s<numStates.getValue(); s++)
		eigValExp[s] = exp(eigenValue[s] * t);
        
    // calculate the transition probabilities
	const double* ptr = &c_ijk[0];
	for (size_t i=0; i<numStates.getValue(); i++) 
		{
		for (size_t j=0; j<numStates.getValue(); j++) 
			{
			double sum = 0.0;
			for(size_t s=0; s<numStates.getValue(); s++)
				sum += (*ptr++) * eigValExp[s];
			P[i][j] = (sum < 0.0) ? 0.0 : sum;
			}
		}
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix::tiProbsComplexEigens(const double t, TransitionProbabilityMatrix& P) const {

    // get a reference to the eigenvalues
    const RbVector<Real>& eigenValueReal = theEigenSystem->getRealEigenvalues();
    const RbVector<Real>& eigenValueComp = theEigenSystem->getImagEigenvalues();

    // precalculate the product of the eigenvalue and the branch length
    std::vector<std::complex<double> > ceigValExp(numStates.getValue());
	for (size_t s=0; s<numStates.getValue(); s++)
        {
        std::complex<double> ev = std::complex<double>(eigenValueReal[s], eigenValueComp[s]);
		ceigValExp[s] = exp(ev * t);
        }

    // calculate the transition probabilities
	const std::complex<double>* ptr = &cc_ijk[0];
	for (size_t i=0; i<numStates.getValue(); i++) 
		{
		for (size_t j=0; j<numStates.getValue(); j++) 
			{
			std::complex<double> sum = std::complex<double>(0.0, 0.0);
			for(size_t s=0; s<numStates.getValue(); s++)
				sum += (*ptr++) * ceigValExp[s];
			P[i][j] = (sum.real() < 0.0) ? 0.0 : sum.real();
			}
		}
}


/** Update the eigen system */
void RateMatrix::updateEigenSystem(void) {

    if (areEigensDirty == false)
        return;
    theEigenSystem->update();
    calculateCijk();
    areEigensDirty = false;
}

