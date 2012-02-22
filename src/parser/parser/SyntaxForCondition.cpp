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


/** Standard constructor */
SyntaxForCondition::SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr) : SyntaxElement(), varName(identifier), inExpression(inExpr), vector(NULL), nextElement(-1) {

    if ( inExpression == NULL ) {
        throw RbException("The 'in' expression of for loop is empty");
    }
}


/** Deep copy constructor */
SyntaxForCondition::SyntaxForCondition(const SyntaxForCondition& x) : SyntaxElement(x) {

    varName                 = new RbString(*(x.varName));
    inExpression            = x.inExpression->clone();
    vector                  = NULL;
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
        vector                   = NULL;
        nextElement              = -1;
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxForCondition::clone () const {

    return (SyntaxElement*)(new SyntaxForCondition(*this));
}


/** Finalize loop. */
void SyntaxForCondition::finalizeLoop(void) {

    if ( nextElement < 0 )
        return;
    
    nextElement = -1;
}


/** Get next loop state */
bool SyntaxForCondition::getNextLoopState(Environment& env) {

    if ( nextElement < 0 )
        initializeLoop( env );
    
    if ( nextElement == static_cast<int>(vector->size()) ) {
        finalizeLoop();
        return false;
    }

    Variable& theVar = env[ *varName ].getVariable();
    // set the new value of the iterator variable
    theVar.setDagNode( new ConstantNode( static_cast<RbLanguageObject*>( vector->getElement( nextElement ).clone() ) ) );
    // the setting of the new iterator might have affected other nodes; therefore we call a keep
    theVar.getDagNode()->keep();
    
    nextElement++;

    return true;
}


/** Get class name of object */
const std::string& SyntaxForCondition::getClassName(void) { 
    
    static std::string rbClassName = "For loop";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxForCondition::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxForCondition::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value (not applicable so return NULL) */
RbVariablePtr SyntaxForCondition::evaluateContent( Environment& env ) {

    return RbVariablePtr( NULL );
}


/** Initialize loop state */
void SyntaxForCondition::initializeLoop(Environment& env) {

    assert ( nextElement < 0 );

    // Evaluate expression and check that we get a vector
    RbDagNodePtr  theNode  = inExpression->evaluateContent(env)->getDagNode();
    const RbObject& theValue = theNode->getValue();

    // Check that it is a vector
    if ( theValue.isTypeSpec( AbstractVector::getClassTypeSpec() ) == false ) {
       throw ( RbException("The 'in' expression does not evaluate to a vector") );
    }
    vector = dynamic_cast<AbstractVector*>(theValue.clone());

    // Initialize nextValue
    nextElement = 0;

    // Add loop variable to frame if it is not there already
    if (!env.existsVariable(*varName)) {
        env.addVariable( *varName );
    }
    
}


/** Print info about syntax element */
void SyntaxForCondition::printValue(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << std::string(*varName) << std::endl;
    o << "inExpression = " << inExpression;
    o << std::endl;

    inExpression->printValue(o);
}

