/**
 * @file
 * This file contains the implementation of SyntaxForCondition, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxForCondition
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "AbstractVector.h"
#include "ConstantNode.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "SyntaxForCondition.h"
#include "VectorNatural.h"
#include "VectorString.h"

#include <cassert>
#include <sstream>


// Definition of the static type spec member
const TypeSpec SyntaxForCondition::typeSpec(SyntaxForCondition_name);

/** Standard constructor */
SyntaxForCondition::SyntaxForCondition(RbPtr<RbString> identifier, RbPtr<SyntaxElement> inExpr) : SyntaxElement(), varName(identifier), inExpression(inExpr), vector(NULL), nextElement(-1) {

    if ( inExpression == NULL ) {
        throw RbException("The 'in' expression of for loop is empty");
    }
}


/** Deep copy constructor */
SyntaxForCondition::SyntaxForCondition(const SyntaxForCondition& x) : SyntaxElement(x) {

    varName                 = RbPtr<RbString>( new RbString(*(x.varName)) );
    inExpression            = RbPtr<SyntaxElement>( x.inExpression->clone() );
    vector                  = RbPtr<AbstractVector>( NULL );
    nextElement             = -1;
}


/** Destructor deletes members */
SyntaxForCondition::~SyntaxForCondition() {
    
}


/** Assignment operator */
SyntaxForCondition& SyntaxForCondition::operator=(const SyntaxForCondition& x) {

    if (&x != this) {
    
        SyntaxElement::operator=(x);

        varName                  = x.varName;
        inExpression             = x.inExpression;
        vector                   = RbPtr<AbstractVector>::getNullPtr();
        nextElement              = -1;
    }

    return *this;
}


/** Return brief info about object */
std::string SyntaxForCondition::briefInfo () const {

    std::ostringstream   o;

    o << "SyntaxForCondition: variable = '" << std::string(*varName);
    o << "', in expression = ";
    o << inExpression->briefInfo();

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxForCondition::clone () const {

    return (SyntaxElement*)(new SyntaxForCondition(*this));
}


/** Finalize loop. */
void SyntaxForCondition::finalizeLoop(RbPtr<Environment> env) {

    if ( nextElement < 0 )
        return;
    
    nextElement = -1;
}


/** Get next loop state */
bool SyntaxForCondition::getNextLoopState(RbPtr<Environment> env) {

    if ( nextElement < 0 )
        initializeLoop( env );
    
    if ( nextElement == static_cast<int>(vector->size()) ) {
        finalizeLoop( env );
        return false;
    }

    (*env)[ *varName ]->getVariable()->setDagNode( RbPtr<DAGNode>( new ConstantNode(RbPtr<RbLanguageObject>(static_cast<RbLanguageObject*>(vector->getElement( nextElement ).get() ) ) ) ) );
    nextElement++;

    return true;
}


/** Get class vector describing type of object */
const VectorString& SyntaxForCondition::getClass(void) const { 
    
    static VectorString rbClass = VectorString(SyntaxForCondition_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value (not applicable so return NULL) */
RbPtr<Variable> SyntaxForCondition::evaluateContent(RbPtr<Environment> env) {

    return RbPtr<Variable>::getNullPtr();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& SyntaxForCondition::getTypeSpec(void) const {
    return typeSpec;
}


/** Initialize loop state */
void SyntaxForCondition::initializeLoop(RbPtr<Environment> env) {

    assert ( nextElement < 0 );

    // Evaluate expression and check that we get a vector
    RbPtr<DAGNode> theNode = inExpression->evaluateContent(env)->getDagNode();
    RbPtr<RbLanguageObject> theValue( theNode->getValue() );

    // Check that it is a vector
    if ( theValue->isTypeSpec( TypeSpec(AbstractVector_name) ) == false ) {
       throw ( RbException("The 'in' expression does not evaluate to a vector") );
    }
    vector = RbPtr<AbstractVector>( dynamic_cast<AbstractVector*>(theValue.get()) );

    // Initialize nextValue
    nextElement = 0;

    // Add loop variable to frame if it is not there already
    if (!env->existsVariable(*varName)) {
        env->addVariable( *varName, TypeSpec( vector->getElementType() ) );
    }
    
}


/** Print info about syntax element */
void SyntaxForCondition::print(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << std::string(*varName) << std::endl;
    o << "inExpression = " << inExpression;
    o << std::endl;

    inExpression->print(o);
}

