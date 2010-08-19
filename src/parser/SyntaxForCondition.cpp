/**
 * @file
 * This file contains the implementation of SyntaxForCondition, which is
 * used to hold formal argument specifications in the syntax tree.
 *
 * @brief Implementation of SyntaxForCondition
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxForCondition.h"

#include <cassert>
#include <sstream>


/** Standard constructor */
SyntaxForCondition::SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr)
    : SyntaxElement(), varName(identifier), inExpression(inExpr), isLoopInitialized(false), vector(NULL) {

    if (inExpression == NULL) {
        delete varName;
        throw RbException("The 'in' expression of for loop is empty");
    }
}


/** Deep copy constructor */
SyntaxForCondition::SyntaxForCondition(const SyntaxForCondition& x)
    : SyntaxElement(x) {

    varName             = new RbString(*(x.varName));
    inExpression        = x.inExpression->clone();
    isLoopInitialized   = false;
    vector              = NULL;
}


/** Destructor deletes members */
SyntaxForCondition::~SyntaxForCondition() {
    
    delete varName;
    delete inExpression;
    if (isLoopInitialized)
        delete vector;
}


/** Return brief info about object */
std::string SyntaxForCondition::briefInfo () const {

    std::ostringstream   o;

    o << "SyntaxForCondition: variable = '" << std::string(*varName);
    o << "', in expression = ";
    RbObject* inValue = inExpression->getValue();
    inValue->printValue(o);
    delete inValue;

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxForCondition::clone () const {

    return (SyntaxElement*)(new SyntaxForCondition(*this));
}


/** Equals comparison */
bool SyntaxForCondition::equals(const SyntaxElement* elem) const {

	const SyntaxForCondition* p = dynamic_cast<const SyntaxForCondition*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && varName->equals(p->varName);
    result = result && inExpression->equals(p->inExpression);

    return result;
}


/** Convert element to DAG node (not applicable so return NULL) */
DAGNode* SyntaxForCondition::getDAGNode(Frame* frame) const {

    return NULL;
}


/** Initialize loop state */
void SyntaxForCondition::initializeLoop(Frame* frame) {

    assert (!isLoopInitialized);

    // Evaluate expression and check that we get a vector
    RbObject* value = inExpression->getValue(frame);

    // Check that it is a vector of Container type
    vector = NULL;
    intVector = NULL;
    if ( value->getDim() != 1 )
        throw ( RbException("The 'in' expression does not evaluate to a vector") );
    vector = dynamic_cast<Container*>(value);
    if ( !vector ) {
        // throw ( RbException("The 'in' expression does not evaluate to a container") );
        intVector = dynamic_cast<VectorInteger*>(value);
        if ( !intVector )
            throw ( RbException("The 'in' expression does not evaluate to a container or int vector") );
    }

    // Initialize nextValue
    nextElement = 0;

    // Add loop variable to frame if it is not there already
    if ( !frame->existsVariable( *varName ) ) {
        if (vector != NULL)
            frame->addVariable( *varName, vector->getElementType() );
        else
            frame->addVariable( *varName, intVector->getElementType() );
    }

    isLoopInitialized = true;
}


/** Get next loop state */
bool SyntaxForCondition::getNextLoopState(Frame* frame) {

    if (!isLoopInitialized)
        initializeLoop(frame);
    
    if ( (vector != NULL && nextElement == vector->size()) || (intVector != NULL && nextElement == intVector->size()) )
        return false;

    if (vector != NULL)
        frame->setVariable( *varName, new ConstantNode(vector->getElement(nextElement)->clone()) );
    else if (intVector != NULL)
        frame->setVariable( *varName, new ConstantNode(intVector->getElement(nextElement)->clone()) );

    nextElement++;

    return true;
}


/** Get semantic value (not applicable so return NULL) */
RbObject* SyntaxForCondition::getValue(Frame* frame) const {

    return NULL;
}


/** Print info about syntax element */
void SyntaxForCondition::print(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << std::string(*varName) << std::endl;
    o << "inExpression = ";
    RbObject* temp = inExpression->getValue();
    temp->printValue(o);
    delete temp;
    o << std::endl;
}


