/**
 * @file
 * This file contains the implementation of SyntaxConstant, which is
 * used to hold constants in the syntax tree.
 *
 * @brief Implementation of SyntaxConstant
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include <iostream>

#include "ConstantNode.h"
#include "RbLanguageObject.h"
#include "SyntaxConstant.h"


/** Construct from value */
SyntaxConstant::SyntaxConstant(RbLanguageObject* val) : SyntaxElement(), value(val) {
}


/** Deep copy constructor */
SyntaxConstant::SyntaxConstant(const SyntaxConstant& x) : SyntaxElement(x), value(NULL) {

    if (value != NULL) {
        value = (x.value->clone());
        value->retain();
    }
}


/** Destructor deletes value */
SyntaxConstant::~SyntaxConstant(void) {
    
    if (value != NULL) {
        value->release();
        if (value->isUnreferenced()) {
            delete value;
        }
    }
}


/** Assignment operator deletes value and makes a clone of the value */
SyntaxConstant& SyntaxConstant::operator=(const SyntaxConstant& x) {

    if (this != &x) {

        SyntaxElement::operator=(x);
        
        if (value != NULL) {
            value->release();
            if (value->isUnreferenced()) {
                delete value;
            }
            value = NULL;
        }

        if (x.value != NULL) {
            value = x.value->clone();
            value->retain();
        }
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


/** Get semantic value of element */
Variable* SyntaxConstant::getContentAsVariable(Environment* env) const {

    // We return a clone in case this function is called repeatedly. The ConstantNode manages the clone.
    if (value == NULL)
        return new Variable(new ConstantNode(NULL));
    else
        return new Variable(new ConstantNode(value));
}


/** Print info about the syntax element */
void SyntaxConstant::print(std::ostream& o) const {

    o << "SyntaxConstant: value = " << value->briefInfo() << std::endl;
}

