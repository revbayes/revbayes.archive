/**
 * @file
 * This file contains the implementation of RbFunction_sqrt, the
 * sqrt() function.
 *
 * @brief Implementation of RbFunction_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Fredrik Ronquist and the REvBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#include "RbFunction_GTR.h"
#include "RbCharacterMatrix.h"
#include "RbDnaAlignment.h"
#include "RbDouble.h"
#include "RbObject.h"
#include "DAGNode.h"
#include "RbException.h"
#include "StringVector.h"
#include "RbString.h"
#include "RbNames.h"
#include "RbUndefined.h"
#include <cmath>

/** Define the argument rules */

/** Add to symbol table */
//static bool fxn_ReadAlignment = SymbolTable::globalTable().add("sqrt", new RbFunction_GTR());


/** Default constructor, allocate workspace */
RbFunction_GTR::RbFunction_GTR(void) :
    RbFunction() {

}

/** Copy constructor */
RbFunction_GTR::RbFunction_GTR(
        const RbFunction_GTR& s) :
    RbFunction(s) {

}

/** Destructor, delete workspace */
RbFunction_GTR::~RbFunction_GTR() {

}

/**
 * @brief clone function
 *
 * This function creates a deep copy of this object.
 *
 * @see RbObject.clone()
 * @returns           return a copy of this object
 *
 */
RbObject* RbFunction_GTR::clone(void) const {

    RbObject *x = new RbFunction_GTR(*this);
    return x;
}

std::string RbFunction_GTR::toString(void) const {

    return RbNames::GTR::name;
}

/** Execute function */
const RbObject* RbFunction_GTR::executeOperation(
        const std::vector<DAGNode*>& arguments) const {

    return NULL;
}

/** Get class vector describing type of object */
const StringVector& RbFunction_GTR::getClass(void) const {

    static StringVector rbClass = StringVector(RbNames::GTR::name) + RbFunction::getClass();
    return rbClass;
}

/** Get the return type */
const std::string RbFunction_GTR::getReturnType(void) const {

    const static std::string returnType = RbNames::Matrix::name;
    return returnType;
}

/** Get the argument rules */
const ArgumentRule** RbFunction_GTR::getArgumentRules(void) const {

    const static ArgumentRule* argRules[] = { NULL };
    return argRules;
}
