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

#include "RbFunction_readCharacterMatrix.h"
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
//static bool fxn_ReadAlignment = SymbolTable::globalTable().add("sqrt", new RbFunction_readCharacterMatrix());


/** Default constructor, allocate workspace */
RbFunction_readCharacterMatrix::RbFunction_readCharacterMatrix(void) :
    RbFunction() {

}

/** Copy constructor */
RbFunction_readCharacterMatrix::RbFunction_readCharacterMatrix(
        const RbFunction_readCharacterMatrix& s) :
    RbFunction(s) {

}

/** Destructor, delete workspace */
RbFunction_readCharacterMatrix::~RbFunction_readCharacterMatrix() {

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
RbObject* RbFunction_readCharacterMatrix::clone(void) const {

    RbObject *x = new RbFunction_readCharacterMatrix(*this);
    return x;
}

std::string RbFunction_readCharacterMatrix::toString(void) const {

    return RbNames::ReadAlignment::name;
}

/** Execute function */
const RbObject* RbFunction_readCharacterMatrix::executeOperation(
        const std::vector<RbObjectWrapper*>& arguments) const {

    /* Get actual argument */
    std::string fName = ((RbString*) arguments[0]->getValue())->getValue();
    std::string fType = ((RbString*) arguments[1]->getValue())->getValue();
    std::string aType = ((RbString*) arguments[2]->getValue())->getValue();

    RbCharacterMatrix* m;
    if (aType == RbNames::AlignmentType::dna)
        m = new RbDnaAlignment(fName, fType);
    else if (aType == RbNames::AlignmentType::aa)
        ;

    return m;
}

/** Get the return type */
const std::string RbFunction_readCharacterMatrix::getReturnType(void) const {

    const static std::string returnType = RbNames::CharacterMatrix::name;
    return returnType;
}

/** Get the argument rules */
const ArgumentRule** RbFunction_readCharacterMatrix::getArgumentRules(void) const {

    const static ArgumentRule* argRules[] = { new ArgumentRule(
            RbNames::ReadAlignment::fileName, RbNames::RbString::name,
            new RbString(""), new RbUndefined, new RbUndefined),
            new ArgumentRule(RbNames::ReadAlignment::fileType,
                    RbNames::RbString::name, new RbString(""), new RbUndefined,
                    new RbUndefined), new ArgumentRule(
                    RbNames::ReadAlignment::alignmentType,
                    RbNames::RbString::name, new RbString(""), new RbUndefined,
                    new RbUndefined), NULL };
    return argRules;
}
