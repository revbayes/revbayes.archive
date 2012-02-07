/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition probabilities in RevBayes.
 *
 * @brief Implementation of TransitionProbabilityMatrix
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


// Definition of the static type spec member
const TypeSpec TransitionProbabilityMatrix::typeSpec(TransitionProbabilityMatrix_name);

/** Constructor passes member rules and method inits to base class */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(void) : ConstantMemberObject(getMemberRules()) {

    numStates = 2;
    theMatrix = new MatrixReal(numStates, numStates);
}


/** Construct rate matrix with n states */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(size_t n) : ConstantMemberObject(getMemberRules()) {

    numStates = n;
    theMatrix = new MatrixReal(numStates, numStates);
}


/** Copy constructor */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m) {

    numStates = m.numStates;
    theMatrix = m.theMatrix->clone();
}


/** Destructor */
TransitionProbabilityMatrix::~TransitionProbabilityMatrix(void) {
    
}


/** Index operator (const) */
const VectorReal* TransitionProbabilityMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates )
        throw RbException( "Index to " + TransitionProbabilityMatrix_name + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Index operator */
VectorReal* TransitionProbabilityMatrix::operator[]( const size_t i ) {

    if ( i >= numStates )
        throw RbException( "Index to " + TransitionProbabilityMatrix_name + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Clone object */
TransitionProbabilityMatrix* TransitionProbabilityMatrix::clone(void) const {

    return new TransitionProbabilityMatrix(*this);
}


/** Map calls to member methods */
RbLanguageObject* TransitionProbabilityMatrix::executeOperationSimple(const std::string& name, Environment* args) {

    if (name == "nstates") {
        return new Natural( (int)numStates );
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class vector describing type of object */
const VectorString& TransitionProbabilityMatrix::getClass(void) const {

    static VectorString rbClass = VectorString(TransitionProbabilityMatrix_name) + MemberObject::getClass();
    return rbClass;
}


/** Get member rules */
const MemberRules* TransitionProbabilityMatrix::getMemberRules(void) const {

    static MemberRules* memberRules = new MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Get methods */
const MethodTable* TransitionProbabilityMatrix::getMethods(void) const {

    static MethodTable* methods = new MethodTable();
    static ArgumentRules* nstatesArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        methods->addFunction("nstates", new MemberFunction(Natural_name, nstatesArgRules) );
        
        // necessary call for proper inheritance
        methods->setParentTable( MemberObject::getMethods() );
        methodsSet = true;
        }

    return methods;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& TransitionProbabilityMatrix::getTypeSpec(void) const {
    return typeSpec;
}


/** Print value for user */
void TransitionProbabilityMatrix::printValue(std::ostream& o) const {

    o << "Transition probability matrix:" << std::endl;
    theMatrix->printValue( o );
    o << std::endl;
}


/** Complete info */
std::string TransitionProbabilityMatrix::richInfo(void) const {

	std::ostringstream o;
    printValue( o );
    return o.str();
}

