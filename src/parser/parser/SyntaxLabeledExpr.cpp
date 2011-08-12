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
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Environment.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "SyntaxLabeledExpr.h"
#include "VectorString.h"

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


/** Assignment operator */
SyntaxLabeledExpr& SyntaxLabeledExpr::operator=(const SyntaxLabeledExpr& x) {

    if (&x != this) {

        delete label;
        delete expression;

        SyntaxElement::operator=(x);

        label      = new RbString(*(x.label));
        expression = x.expression->clone();
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxLabeledExpr::briefInfo () const {

    std::ostringstream   o;

    if (expression == NULL)
        o << "SyntaxLabeledExpr:  '" << std::string(*label) << "' = <empty>";
    else {
        o << "SyntaxLabeledExpr:  '" << std::string(*label) << "' = ";
        o << expression->briefInfo();
    }

    return o.str();
}


/** Clone syntax element */
SyntaxLabeledExpr* SyntaxLabeledExpr::clone () const {

    return new SyntaxLabeledExpr(*this);
}


/** Get class vector describing type of object */
const VectorString& SyntaxLabeledExpr::getClass(void) const { 

    static VectorString rbClass = VectorString(SyntaxLabeledExpr_name) + SyntaxElement::getClass();
	return rbClass; 
}


/** Get semantic value (not applicable so return NULL) */
Variable* SyntaxLabeledExpr::getContentAsVariable(Environment* env) const {

    return NULL;
}


/** Print info about the syntax element */
void SyntaxLabeledExpr::print(std::ostream& o) const {

    o << "[" << this << "] SyntaxLabeledExpr:" << std::endl;
    o << "label      = " << label->richInfo() << std::endl;
    o << "expression = [" << expression << "]" << expression->briefInfo() << std::endl;
    o << std::endl;

    expression->print(o);
}

