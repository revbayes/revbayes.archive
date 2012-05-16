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

#include "ConstantNode.h"
#include "Container.h"
#include "Environment.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "SyntaxForCondition.h"

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

/** Get semantic value (not applicable so return NULL) */
RbPtr<Variable> SyntaxForCondition::evaluateContent( Environment& env ) {
    
    return NULL;
}


/** Finalize loop. */
void SyntaxForCondition::finalizeLoop(void) {

    if ( nextElement < 0 )
        return;
    
    nextElement = -1;
}


/** Get the name of the index variable */
const RbString& SyntaxForCondition::getIndexVarName( void ) const {
    return *varName;
}


/** Get next loop state */
RbLanguageObject& SyntaxForCondition::getNextLoopState( void ) {

    // We do not check here for the loop initialization or finalization. The caller needs to do some work! (Sebastian)
//    if ( nextElement < 0 )
//        initializeLoop( env );
//    
//    if ( nextElement == static_cast<int>(vector->size()) ) {
//        finalizeLoop();
//        return false;
//    }

    // We do not have the index variable inserted in the environment but replace it with a constant (Sebastian)
//    Variable& theVar = env[ *varName ].getVariable();
//    // set the new value of the iterator variable
//    theVar.setDagNode( new ConstantNode( static_cast<RbLanguageObject*>( vector->getElement( nextElement ).clone() ) ) );
//    // the setting of the new iterator might have affected other nodes; therefore we call a keep
//    theVar.getDagNode()->keep();
    
    RbLanguageObject& elm = static_cast<RbLanguageObject&>( vector->getElement( nextElement ) );
    
    nextElement++;

    return elm;
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


/** Have we already reached the end of the loop? */
bool SyntaxForCondition::isFinished( void ) const {
    return nextElement < static_cast<int>(vector->size());
}


/** Initialize loop state */
void SyntaxForCondition::initializeLoop(Environment& env) {

    assert ( nextElement < 0 );

    // Evaluate expression and check that we get a vector
    const RbPtr<Variable>&              theVar      = inExpression->evaluateContent(env);
    const RbPtr<DAGNode>&               theNode     = theVar->getDagNode();
    const RlValue<RbLanguageObject>&    theValue    = theNode->getValue();

    // Check that it is a vector
    if ( theValue.isTypeSpec( Container::getClassTypeSpec() ) == false ) {
       throw ( RbException("The 'in' expression does not evaluate to a vector") );
    }
    vector = dynamic_cast<Container*>(theValue.getSingleValue()->clone());

    // Initialize nextValue
    nextElement = 0;

    // We do not add the index variable because we will replace the variable in all statements (Sebastian)
//    // Add loop variable to frame if it is not there already
//    if (!env.existsVariable(*varName)) {
//        env.addVariable( *varName );
//    }
    
}


/** Print info about syntax element */
void SyntaxForCondition::printValue(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << varName->getValue() << std::endl;
    o << "inExpression = " << inExpression;
    o << std::endl;

    inExpression->printValue(o);
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the inExpression.
 */
void SyntaxForCondition::replaceVariableWithConstant(const std::string& name, const RbLanguageObject& c) {
    
    inExpression->replaceVariableWithConstant(name, c);
    
}

