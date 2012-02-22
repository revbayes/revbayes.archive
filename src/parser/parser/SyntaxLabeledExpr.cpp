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
#include "RbUtil.h"
#include "RbObject.h"
#include "SyntaxLabeledExpr.h"
#include "VectorString.h"

#include <sstream>


/** Constructor from label and expression */
SyntaxLabeledExpr::SyntaxLabeledExpr(RbString* id, SyntaxElement* expr) : SyntaxElement(), label(id), expression(expr) {
}


/** Deep copy constructor */
SyntaxLabeledExpr::SyntaxLabeledExpr(const SyntaxLabeledExpr& x) : SyntaxElement(x) {

    label      = new RbString(*(x.label) );
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

        SyntaxElement::operator=(x);

        label      = x.label->clone();
        expression = x.expression->clone();
    }

    return (*this);
}


/** Clone syntax element */
SyntaxLabeledExpr* SyntaxLabeledExpr::clone () const {

    return new SyntaxLabeledExpr(*this);
}



/** Get class name of object */
const std::string& SyntaxLabeledExpr::getClassName(void) { 
    
    static std::string rbClassName = "Labeled expression";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SyntaxLabeledExpr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( SyntaxElement::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SyntaxLabeledExpr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get semantic value (not applicable so return NULL) */
RbVariablePtr SyntaxLabeledExpr::evaluateContent( Environment& env ) {

    return RbVariablePtr( NULL );
}


/** Print info about the syntax element */
void SyntaxLabeledExpr::printValue(std::ostream& o) const {

    o << "[" << this << "] SyntaxLabeledExpr:" << std::endl;
    o << "label      = " << label->getValue() << std::endl;
    o << "expression = [" << expression << "]";
    expression->printValue(o);
    o << std::endl;
    o << std::endl;

    expression->printValue(o);
}

