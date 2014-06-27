/**
 * @file
 * This file contains the implementation of SyntaxForLoop, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxForLoop
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
#include "RlString.h"
#include "SyntaxForLoop.h"

#include <cassert>
#include <sstream>

using namespace RevLanguage;

/** Standard constructor */
SyntaxForLoop::SyntaxForLoop(const std::string &identifier, SyntaxElement* inExpr) : SyntaxElement(), varName(identifier), inExpression(inExpr), stateSpace(NULL), nextElement(-1) {

    if ( inExpression == NULL ) {
        throw RbException("The 'in' expression of for loop is empty");
    }
}


/** Deep copy constructor */
SyntaxForLoop::SyntaxForLoop(const SyntaxForLoop& x) : SyntaxElement(x) {

    varName                 = x.varName;
    inExpression            = x.inExpression->clone();
    stateSpace              = NULL;
    nextElement             = -1;
}


/** Destructor deletes members */
SyntaxForLoop::~SyntaxForLoop() {
    
    delete inExpression;
    delete stateSpace;
}


/** Assignment operator */
SyntaxForLoop& SyntaxForLoop::operator=(const SyntaxForLoop& x) {

    if (&x != this) {
    
        SyntaxElement::operator=(x);

        varName                  = x.varName;
        inExpression             = x.inExpression->clone();
        stateSpace               = NULL;
        nextElement              = -1;
    }

    return *this;
}


/** Clone syntax element */
SyntaxElement* SyntaxForLoop::clone () const {

    return (SyntaxElement*)(new SyntaxForLoop(*this));
}

/** Get semantic value (not applicable so return NULL) */
RbPtr<Variable> SyntaxForLoop::evaluateContent( Environment& env ) {
    
    return NULL;
}


/** Finalize loop. */
void SyntaxForLoop::finalizeLoop(void) {

    if ( nextElement < 0 )
        return;
    
    nextElement = -1;
}


/** Get the name of the index variable */
const std::string& SyntaxForLoop::getIndexVarName( void ) const {
    return varName;
}


/** Get next loop state */
RevObject* SyntaxForLoop::getNextLoopState( void ) {

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
//    theVar.setDagNode( new ConstantNode( static_cast<RevObject*>( vector->getElement( nextElement ).clone() ) ) );
//    // the setting of the new iterator might have affected other nodes; therefore we call a keep
//    theVar.getDagNode()->keep();
    
    RevObject *elm = stateSpace->getElement( size_t(nextElement) );
    
    nextElement++;

    return elm;
}



/** Have we already reached the end of the loop? */
bool SyntaxForLoop::isFinished( void ) const {
    return nextElement < static_cast<int>(stateSpace->size());
}


/** Initialize loop state */
void SyntaxForLoop::initializeLoop(Environment& env) {

    assert ( nextElement < 0 );

    // Evaluate expression and check that we get a vector
    const RbPtr<Variable>&              theVar      = inExpression->evaluateContent(env);
    const RevObject&             theValue    = theVar->getValue();

    // Check that it is a vector
    if ( theValue.isTypeSpec( Container::getClassTypeSpec() ) == false ) {
       throw ( RbException("The 'in' expression does not evaluate to a vector") );
    }
    stateSpace = dynamic_cast<Container*>(theValue.clone());

    // Initialize nextValue
    nextElement = 0;

    // We do not add the index variable because we will replace the variable in all statements (Sebastian)
//    // Add loop variable to frame if it is not there already
//    if (!env.existsVariable(*varName)) {
//        env.addVariable( *varName );
//    }
    
}


/** Print info about syntax element */
void SyntaxForLoop::printValue(std::ostream& o) const {

    o << "SyntaxForLoop:" << std::endl;
    o << "varName      = " << varName << std::endl;
    o << "inExpression = ";
    inExpression->printValue( o );
    o << std::endl;
}


/**
 * Replace the syntax variable with name by the constant value. Loops have to do that for their index variables.
 * We just delegate that to the inExpression.
 */
void SyntaxForLoop::replaceVariableWithConstant(const std::string& name, const RevObject& c) {
    
    inExpression->replaceVariableWithConstant(name, c);
    
}

