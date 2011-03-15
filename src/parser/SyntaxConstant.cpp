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
#include "RbObject.h"
#include "SyntaxConstant.h"


/** Construct from value */
SyntaxConstant::SyntaxConstant(RbObject* val) : SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& x) : SyntaxElement(x), value(NULL) {

    if (value != NULL)
        value = (x.value->clone());
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant(void) {
    
    if (value != NULL)
        delete value;
}


/** Assignment operator deletes value and makes a clone of the value */
SyntaxConstant& SyntaxConstant::operator=(const SyntaxConstant& x) {

    if (this != &x) {

        SyntaxElement::operator=(x);
        
        if (value != NULL) {
            delete value;
            value = NULL;
        }

        if (x.value != NULL)
            value = x.value->clone();
    }

    return (*this);
}


/** Return brief info about object */
std::string SyntaxConstant::briefInfo (void) const {

    return "SyntaxConstant: value = " + value->briefInfo();
}


/** Clone syntax element */
SyntaxConstant* SyntaxConstant::clone (void) const {

    return new SyntaxConstant(*this);
}


/** Convert element to DAG node expression */
DAGNode* SyntaxConstant::getDAGNodeExpr(Frame* frame) const {

    return getValue(frame);
}


/** Get semantic value of element */
DAGNode* SyntaxConstant::getValue(Frame* frame) const {

    // We return a clone in case this function is called repeatedly. The ConstantNode manages the clone.
    if (value == NULL)
        return new ConstantNode(NULL);
    else
        return new ConstantNode(value->clone());
}


/** Print info about the syntax element */
void SyntaxConstant::print(std::ostream& o) const {

    o << "SyntaxConstant: value = " << value->briefInfo() << std::endl;
}

