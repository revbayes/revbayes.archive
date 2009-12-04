/**
 * @file
 * This file contains the implementation of SyntaxLabeledExpr, which is
 * used to hold labeled expressions that represent arguments to
 * functions in the syntax tree.
 *
 * @brief Implementation of SyntaxLabeledExpr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbObject.h"
#include "SyntaxLabeledExpr.h"
#include "Workspace.h"

#include <sstream>


/** Constructor from label and expression */
SyntaxLabeledExpr::SyntaxLabeledExpr(RbString* id, SyntaxElement* expr)
    : SyntaxElement(), label(id), expression(expr) {
}


/** Deep copy constructor */
SyntaxLabeledExpr::SyntaxLabeledExpr(const SyntaxLabeledExpr& x)
    : SyntaxElement(x) {

    label      = new RbString(*(x.label));
    expression = x.expression->clone();
}


/** Destructor deletes members */
SyntaxLabeledExpr::~SyntaxLabeledExpr() {
    
    delete label;
    delete expression;
}


/** Return brief info about object */
std::string SyntaxLabeledExpr::briefInfo () const {

    std::ostringstream   o;

    if (expression == NULL)
        o << "SyntaxLabeledExpr:  '" << std::string(*label) << "' = <empty>";
    else if (expression->isConstExpr()) {
        o << "SyntaxLabeledExpr:  '" << std::string(*label) << "' = ";
        expression->getValue()->printValue(o);
    }
    else
        o << "SyntaxLabeledExpr:  '" << std::string(*label) << "' = <value>";

    return o.str();
}


/** Clone syntax element */
SyntaxElement* SyntaxLabeledExpr::clone () const {

    return (SyntaxElement*)(new SyntaxLabeledExpr(*this));
}


/** Equals comparison */
bool SyntaxLabeledExpr::equals(const SyntaxElement* elem) const {

	const SyntaxLabeledExpr* p = dynamic_cast<const SyntaxLabeledExpr*>(elem);
    if (p == NULL)
        return false;

    bool result = true;
    result = result && label->equals(p->label);
    result = result && expression->equals(p->expression);

    return result;
}


/** Convert element to DAG node (not applicable so return NULL) */
DAGNode* SyntaxLabeledExpr::getDAGNode(Frame* frame) const {

    return NULL;
}


/** Get semantic value (not applicable so return NULL) */
RbObject* SyntaxLabeledExpr::getValue(Frame* frame) {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxLabeledExpr::print(std::ostream& o) const {

    o << "SyntaxLabeledExpr:" << std::endl;
    o << "label      = " << label->toString() << std::endl;
    o << "expression = " << expression->briefInfo() << std::endl;
}


