/**
 * @file
 * This file contains the implementation of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Implementation of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>

#include "ConstantNode.h"
#include "SyntaxConstant.h"


/** Construct from value */
SyntaxConstant::SyntaxConstant(RbObject* val) :
    SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& sc)
    : SyntaxElement(sc), value(sc.value->clone()) {
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant() {
    delete value;
}


/** Return brief info about object */
std::string SyntaxConstant::briefInfo () const {

    return "SyntaxConstant; value = " + value->briefInfo();
}


/** Clone syntax element */
SyntaxElement* SyntaxConstant::clone () const {

    return (SyntaxElement*)(new SyntaxConstant(*this));
}


/** Equals comparison */
bool SyntaxConstant::equals(const SyntaxElement* elem) const {

	const SyntaxConstant* sc = dynamic_cast<const SyntaxConstant*>(elem);

    if (sc == NULL)
        return false;

    return value->equals(sc->value);
}


/** Convert element to DAG node */
DAGNode* SyntaxConstant::getDAGNode(Environment* env) const {

    return new ConstantNode(value->clone());
}


/** Get semantic value of element */
RbObject* SyntaxConstant::getValue(Environment* env) {

    return value;
}


/** Is this a constant expression? */
bool SyntaxConstant::isConstExpr() const {

    return true;
}

 
/** Print info about the syntax element */
void SyntaxConstant::print(std::ostream& o) const {

    o << "SyntaxConstant: value = " << value->briefInfo() << std::endl;
}


