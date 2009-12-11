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

#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "SyntaxForCondition.h"

#include <sstream>


/** Standard constructor */
SyntaxForCondition::SyntaxForCondition(RbString* identifier, SyntaxElement* inExpr)
    : SyntaxElement(), varName(identifier), inExpression(inExpr) {

    if (inExpression == NULL) {
        delete varName;
        throw RbException("The 'in' expression of for loop empty");
    }
}


/** Deep copy constructor */
SyntaxForCondition::SyntaxForCondition(const SyntaxForCondition& x)
    : SyntaxElement(x) {

    varName         = new RbString(*(x.varName));
    inExpression    = x.inExpression->clone();
}


/** Destructor deletes members */
SyntaxForCondition::~SyntaxForCondition() {
    
    delete varName;
    delete inExpression;
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


/** Get semantic value (not applicable so return NULL) */
RbObject* SyntaxForCondition::getValue(Frame* frame) const {

    return NULL;
}


/** Print info about syntax element */
void SyntaxForCondition::print(std::ostream& o) const {

    o << "SyntaxForCondition:" << std::endl;
    o << "varName      = " << std::string(varName) << std::endl;
    o << "inExpression = ";
    RbObject* temp = inExpression->getValue();
    temp->printValue(o);
    delete temp;
    o << std::endl;
}


