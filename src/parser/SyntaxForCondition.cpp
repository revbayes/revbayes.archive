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

#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxForCondition.h"

#include <cassert>
#include <sstream>


/** Standard constructor */
SyntaxForCondition::SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr)
    : SyntaxElement(), varName(identifier), inExpression(inExpr), vector(NULL),
      wasLoopVariableReference(false), nextElement(-1) {

    if ( inExpression == NULL ) {
        delete varName;
        throw RbException("The 'in' expression of for loop is empty");
    }
}


/** Deep copy constructor */
SyntaxForCondition::SyntaxForCondition(const SyntaxForCondition& x)
    : SyntaxElement(x) {

    varName                  = new RbString(*(x.varName));
    inExpression             = x.inExpression->clone();
    vector                   = NULL;
    wasLoopVariableReference = false;
    nextElement              = -1;
}


/** Destructor deletes members */
SyntaxForCondition::~SyntaxForCondition() {
    
    delete varName;
    delete inExpression;
    if ( nextElement >= 0 )
        delete vector;
}


/** Assignment operator */
SyntaxForCondition& SyntaxForCondition::operator=(const SyntaxForCondition& x) {

    if (&x != this) {
    
        SyntaxElement::operator=(x);

        delete varName;
        delete inExpression;
        if ( nextElement >= 0 )
            delete vector;

        varName                  = new RbString(*(x.varName));
        inExpression             = x.inExpression->clone();
        vector                   = NULL;
        wasLoopVariableReference = false;
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
void SyntaxForCondition::finalizeLoop(Frame* frame) {

    if ( nextElement < 0 )
        return;

    if ( !wasLoopVariableReference )
        frame->getVariableSlot( *varName ).setReferenceFlag( false );

    delete vector;
    
    wasLoopVariableReference = false;
    nextElement = -1;
}


/** Convert element to DAG node (not applicable so return NULL) */
DAGNode* SyntaxForCondition::getDAGNodeExpr(Frame* frame) const {

    return NULL;
}


/** Get next loop state */
bool SyntaxForCondition::getNextLoopState(Frame* frame) {

    if ( nextElement < 0 )
        initializeLoop(frame);
    
    if ( nextElement == vector->size() ) {
        finalizeLoop( frame );
        return false;
    }

    frame->getVariableSlot( *varName ).setReference( vector->getElement( nextElement )->clone() );
    nextElement++;

    return true;
}


/** Get semantic value (not applicable so return NULL) */
DAGNode* SyntaxForCondition::getValue(Frame* frame) const {

    return NULL;
}


/** Initialize loop state */
void SyntaxForCondition::initializeLoop(Frame* frame) {

    assert ( nextElement < 0 );

    // Evaluate expression and check that we get a vector
    DAGNode* value = inExpression->getValue(frame);

    // Check that it is a vector
    if ( value->isType( DAGNodePlate_name ) == false || value->getDim() != 1 ) {
        delete value;
        throw ( RbException("The 'in' expression does not evaluate to a vector") );
    }
    vector = dynamic_cast<DAGNodePlate*>(value);

    // Initialize nextValue
    nextElement = 0;

    // Add loop variable to frame if it is not there already; make sure it is a reference variable
    if ( frame->existsVariable( *varName ) )
        frame->getVariableSlot( *varName ).setReferenceFlag( true );
    else
        frame->addReference( *varName, vector->getType(), vector->getDim() );
}


/** Print info about syntax element */
void SyntaxForCondition::print(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << std::string(*varName) << std::endl;
    o << "inExpression = " << inExpression;
    o << std::endl;

    inExpression->print(o);
}

