/**
 * @file
 * This file contains the implementation of RateMatrix, which is
 * class that holds a rate matrix in RevBayes.
 *
 * @brief Implementation of RateMatrix
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package distributions
 *
 * $Id: Mcmc.h 211 2009-12-29 22:23:09Z ronquist $
 */

#include "RbBoolean.h"
#include "EigenSystem.h"
#include "RateMatrix.h"
#include "MatrixReal.h"
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
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"
#include "Workspace.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>


// Definition of the static type spec member
const TypeSpec RateMatrix::typeSpec(RateMatrix_name);

/** Constructor passes member rules and method inits to base class */
RateMatrix::RateMatrix(void) : MemberObject(getMemberRules()) {

    numStates            = 2;
    areEigensDirty       = true;
    reversibilityChecked = false;
    isReversible         = false;
    theRateMatrix        = RbPtr<MatrixReal>( new MatrixReal(numStates, numStates, 0.0) );
    theStationaryFreqs   = RbPtr<Simplex>( new Simplex(numStates) );
    theEigenSystem       = RbPtr<EigenSystem>( new EigenSystem(theRateMatrix) );
    c_ijk.resize(numStates*numStates*numStates);
    cc_ijk.resize(numStates*numStates*numStates);
}


/** Construct rate matrix with n states */
RateMatrix::RateMatrix(size_t n) : MemberObject(getMemberRules()) {

    numStates            = n;
    areEigensDirty       = true;
    reversibilityChecked = false;
    isReversible         = false;
    theRateMatrix        = RbPtr<MatrixReal>( new MatrixReal(numStates, numStates, 0.0) );
    theStationaryFreqs   = RbPtr<Simplex>( new Simplex(numStates) );
    theEigenSystem       = RbPtr<EigenSystem>( new EigenSystem(theRateMatrix) );
    c_ijk.resize(numStates*numStates*numStates);
    cc_ijk.resize(numStates*numStates*numStates);
}


/** Copy constructor */
RateMatrix::RateMatrix(const RateMatrix& m) {

    numStates            = m.numStates;
    areEigensDirty       = m.areEigensDirty;
    reversibilityChecked = m.reversibilityChecked;
    isReversible         = m.isReversible;
    theRateMatrix        = RbPtr<MatrixReal>( m.theRateMatrix->clone() );
    theStationaryFreqs   = RbPtr<Simplex>( m.theStationaryFreqs->clone() );
    theEigenSystem       = RbPtr<EigenSystem>( m.theEigenSystem->clone() );
    c_ijk                = m.c_ijk;
    cc_ijk               = m.cc_ijk;
    theEigenSystem->setRateMatrixPtr(theRateMatrix);
}


/** Destructor */
RateMatrix::~RateMatrix(void) {
    
}


/** Index operator (const) */
RbPtr<const VectorReal> RateMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates )
        throw RbException( "Index to " + RateMatrix_name + "[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Index operator */
RbPtr<VectorReal> RateMatrix::operator[]( const size_t i ) {

    if ( i >= numStates )
        throw RbException( "Index to " + RateMatrix_name + "[][] out of bounds" );
    return (*theRateMatrix)[i];
}


/** Calculate the average rate for the rate matrix */
double RateMatrix::averageRate(void) const {

    double ave = 0.0;
    for (size_t i=0; i<numStates; i++)
        ave += -(*theStationaryFreqs)[i] * (*(*theRateMatrix)[i])[i];
    return ave;
}


/** Do precalculations on eigenvectors */
void RateMatrix::calculateCijk(void) {

    if ( theEigenSystem->isComplex() == false )
        {
        // real case
        const MatrixReal& ev  = theEigenSystem->getEigenvectors();
        const MatrixReal& iev = theEigenSystem->getInverseEigenvectors();
        double* pc = &c_ijk[0];
        for (size_t i=0; i<numStates; i++)
            for (size_t j=0; j<numStates; j++)
                for (size_t k=0; k<numStates; k++)
                    *(pc++) = (*ev[i])[k] * (*iev[k])[j];   
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
                    *(pc++) = (*cev[i])[k] * (*ciev[k])[j];
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
    MatrixReal QT(numStates, numStates);
    for (size_t i=0; i<numStates; i++)
        for (size_t j=0; j<numStates; j++)
            (*QT[i])[j] = (*(*theRateMatrix)[j])[i];

	// compute the LU decomposition of the transposed rate matrix
    MatrixReal L(numStates, numStates);
    MatrixReal U(numStates, numStates);
	RbMath::computeLandU(QT, L, U);
	
	// back substitute into z = 0 to find un-normalized stationary frequencies, starting with x_n = 1.0
    VectorReal pi(numStates, 0.0);
	pi[numStates-1] = 1.0;
	for (int i=(int)numStates-2; i>=0; i--)
		{
		double dotProduct = 0.0;
		for (size_t j=i+1; j<numStates; j++)
			dotProduct += (*U[i])[j] * pi[j];
		pi[i] = (0.0 - dotProduct) / (*U[i])[i];
		}
		
	// normalize the solution vector
	double sum = 0.0;
	for (size_t i=0; i<numStates; i++)
		sum += pi[i];
	for (size_t i=0; i<numStates; i++)
		pi[i] /= sum;
        
    // set the stationary frequencies
    theStationaryFreqs->setValue(pi);
}


/** Calculate the transition probabilities */
void RateMatrix::calculateTransitionProbabilities(double t, RbPtr<TransitionProbabilityMatrix> P) const {

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
	for (size_t i=0; i<numStates; i++)
		for (size_t j=i+1; j<numStates; j++)
			diff += fabs( (*theStationaryFreqs)[i] * (*(*theRateMatrix)[i])[j] - (*theStationaryFreqs)[j] * (*(*theRateMatrix)[j])[i] );
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
RbPtr<RbObject> RateMatrix::executeOperation(const std::string& name, const RbPtr<Environment>& args) {

    if (name == "nstates") 
        {
        return RbPtr<RbObject>( new Natural((int)numStates) );
        }
    else if (name == "stationaryfreqs")
        {
        RbPtr<RbObject> s( theStationaryFreqs->clone() );
        return s;
        }
    else if (name == "averate")
        {
        return RbPtr<RbObject>( new RealPos(averageRate()) );
        }        
    else if (name == "reversible")
        {
        return RbPtr<RbObject>( new RbBoolean(isReversible) );
        }        

    return MemberObject::executeOperation( name, args );
}


/** Get class vector describing type of object */
const VectorString& RateMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(RateMatrix_name) + MemberObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& RateMatrix::getTypeSpec(void) const {
    return typeSpec;
}


/** Return whether or not the rate matrix is time reversible */
bool RateMatrix::getIsTimeReversible(void) {

    if (reversibilityChecked == false)
        return checkTimeReversibity(0.000001);
    return isReversible;
}


/** Get member rules */
RbPtr<const MemberRules> RateMatrix::getMemberRules(void) const {

    static RbPtr<MemberRules> memberRules( new MemberRules() );
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return RbPtr<const MemberRules>( memberRules );
}


/** Get methods */
RbPtr<const MethodTable> RateMatrix::getMethods(void) const {

    static RbPtr<MethodTable> methods( new MethodTable() );
    static RbPtr<ArgumentRules> nstatesArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> stationaryfreqsArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> averateArgRules( new ArgumentRules() );
    static RbPtr<ArgumentRules> reversibleArgRules( new ArgumentRules() );
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        methods->addFunction("nstates",         RbPtr<RbFunction>( new MemberFunction(Natural_name, nstatesArgRules)         ) );
        methods->addFunction("stationaryfreqs", RbPtr<RbFunction>( new MemberFunction(Simplex_name, stationaryfreqsArgRules) ) );
        methods->addFunction("averate",         RbPtr<RbFunction>( new MemberFunction(RealPos_name, averateArgRules)         ) );
        methods->addFunction("reversible",      RbPtr<RbFunction>( new MemberFunction(RbBoolean_name, reversibleArgRules)    ) );
        
        // necessary call for proper inheritance
        methods->setParentTable( RbPtr<const FunctionTable>( MemberObject::getMethods() ) );
        methodsSet = true;
        }

    return RbPtr<const MethodTable>( methods );
}


/** Print value for user */
void RateMatrix::printValue(std::ostream& o) const {

    o << "Rate matrix:" << std::endl;
    o << theRateMatrix->richInfo() << std::endl;
    o << "Stationary frequencies: ";
    o << theStationaryFreqs->richInfo() << std::endl;
}


/** Rescale the rates such that the average rate is r */
void RateMatrix::rescaleToAverageRate(const double r) {

    double curAve = averageRate();
    double scaleFactor = r / curAve;
    for (size_t i=0; i<numStates; i++)
        for (size_t j=0; j<numStates; j++)
            (*(*theRateMatrix)[i])[j] *= scaleFactor;
}


/** Complete info */
std::string RateMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}


/** Set the diagonal of the rate matrix such that each row sums to zero */
void RateMatrix::setDiagonal(void) {

    for (size_t i=0; i<numStates; i++)
        {
        double sum = 0.0;
        for (size_t j=0; j<numStates; j++)
            {
            if (i != j)
                sum += (*(*theRateMatrix)[i])[j];
            }
        (*(*theRateMatrix)[i])[i] = -sum;
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
void RateMatrix::tiProbsEigens(const double t, RbPtr<TransitionProbabilityMatrix> P) const {

    // get a reference to the eigenvalues
    const VectorReal& eigenValue = theEigenSystem->getRealEigenvalues();
    
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
			(*(*P)[i])[j] = (sum < 0.0) ? 0.0 : sum;
			}
		}
}


/** Calculate the transition probabilities for the complex case */
void RateMatrix::tiProbsComplexEigens(const double t, RbPtr<TransitionProbabilityMatrix> P) const {

    // get a reference to the eigenvalues
    const VectorReal& eigenValueReal = theEigenSystem->getRealEigenvalues();
    const VectorReal& eigenValueComp = theEigenSystem->getImagEigenvalues();

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
			(*(*P)[i])[j] = (sum.real() < 0.0) ? 0.0 : sum.real();
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

