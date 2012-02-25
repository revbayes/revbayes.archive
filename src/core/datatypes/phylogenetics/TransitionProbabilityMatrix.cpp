/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition probabilities in RevBayes.
 *
 * @brief Implementation of TransitionProbabilityMatrix
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


/** Constructor passes member rules and method inits to base class */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(void) : ConstantMemberObject(getMemberRules()) {

    numStates = 2;
    theMatrix = new MatrixReal(numStates.getValue(), numStates.getValue());
}


/** Construct rate matrix with n states */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(size_t n) : ConstantMemberObject(getMemberRules()) {

    numStates = n;
    theMatrix = new MatrixReal(numStates.getValue(), numStates.getValue());
}


/** Copy constructor */
TransitionProbabilityMatrix::TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m) {

    numStates = m.numStates;
    theMatrix = m.theMatrix->clone();
}


/** Destructor */
TransitionProbabilityMatrix::~TransitionProbabilityMatrix(void) {
    
    delete theMatrix;
}


TransitionProbabilityMatrix& TransitionProbabilityMatrix::operator=(const TransitionProbabilityMatrix &m) {
    
    if (this != &m) {
        ConstantMemberObject::operator=(m);
        
        delete theMatrix;
        
        numStates = m.numStates;
        theMatrix = m.theMatrix->clone();
    }
    
    return *this;
}


/** Index operator (const) */
const VectorReal& TransitionProbabilityMatrix::operator[]( const size_t i ) const {

    if ( i >= numStates.getValue() )
        throw RbException( "Index to " + getClassName() + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Index operator */
VectorReal& TransitionProbabilityMatrix::operator[]( const size_t i ) {

    if ( i >= numStates.getValue() )
        throw RbException( "Index to " + getClassName() + "[][] out of bounds" );
    return (*theMatrix)[i];
}


/** Clone object */
TransitionProbabilityMatrix* TransitionProbabilityMatrix::clone(void) const {

    return new TransitionProbabilityMatrix(*this);
}


/** Map calls to member methods */
const RbLanguageObject& TransitionProbabilityMatrix::executeOperationSimple(const std::string& name, const std::vector<Argument>& args) {

    if (name == "nstates") {
        return numStates;
    }

    return MemberObject::executeOperationSimple( name, args );
}


/** Get class name of object */
const std::string& TransitionProbabilityMatrix::getClassName(void) { 
    
    static std::string rbClassName = "Transition probability matrix";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TransitionProbabilityMatrix::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( ConstantMemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& TransitionProbabilityMatrix::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get member rules */
const MemberRules& TransitionProbabilityMatrix::getMemberRules(void) const {

    static MemberRules memberRules = MemberRules();
    static bool        rulesSet = false;

    if (!rulesSet) 
        {
        rulesSet = true;
        }

    return memberRules;
}


/** Get methods */
const MethodTable& TransitionProbabilityMatrix::getMethods(void) const {

    static MethodTable methods = MethodTable();
    static ArgumentRules* nstatesArgRules = new ArgumentRules();
    static bool          methodsSet = false;

    if ( methodsSet == false ) 
        {
        
        methods.addFunction("nstates", new MemberFunction(Natural::getClassTypeSpec(), nstatesArgRules) );
        
        // necessary call for proper inheritance
        methods.setParentTable( &MemberObject::getMethods() );
        methodsSet = true;
        }

    return methods;
}


/** Print value for user */
void TransitionProbabilityMatrix::printValue(std::ostream& o) const {

    o << "Transition probability matrix:" << std::endl;
    theMatrix->printValue( o );
    o << std::endl;
}

