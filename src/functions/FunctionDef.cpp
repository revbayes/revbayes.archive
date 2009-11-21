/**
 * @file
 * This file contains the implementation of FunctionDef, which is
 * used to describe class member functions.
 *
 * @brief Implementation of FunctionDef
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include <iostream>

#include "FunctionDef.h"
#include "RbException.h"

//static StringVector FunctionDef::rbClass = StringVector("functionDef") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from a type and an (optional) default
 * value.
 *
 * @param v     Value of the object
 *
 */
FunctionDef::FunctionDef(const std::string& name, RbFunction* func)
    : RbObject(), label(name), function(func) {
}


/**
 * @brief Constructor
 *
 * Creates an instance from a type and an (optional) default
 * value.
 *
 * @param v     Value of the object
 *
 */
FunctionDef::FunctionDef(const FunctionDef& fd)
    : RbObject(), label(fd.label), function(fd.function) {
}

/**
 * @brief Brief info about the object
 *
 * Provides brief info about the object in a string.
 *
 * @returns     Short string describing object
 *
 */
std::string FunctionDef::briefInfo() const {

	std::ostringstream o;
    o << "functionDef('" << label << "', " << function->briefInfo() << ")";
    return o.str();
}


/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject. It
 * returns equal only if the label is the same and the functions
 * compare as being equal.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool FunctionDef::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const FunctionDef* x = dynamic_cast<const FunctionDef*>(obj);
    if (x != NULL)
        return (label == x->label && function->equals(x->getFunction()));

    // Try converting the value to an functionDef
    x = dynamic_cast<const FunctionDef*>(obj->convertTo("functionDef"));
    if (x == NULL)
        return false;

    bool result = (label == x->label && function->equals(x->getFunction()));
    delete x;
    return result;
}


/**
 * @brief Print complete info
 *
 * Prints complete object info.
 *
 * @param o     The stream for printing
 *
 */
void FunctionDef::print(std::ostream &o) const {

	RbObject::print(o);
    o << "Label = " << label << std::endl;
    o << "Function = " << function->briefInfo() << std::endl;
}


/**
 * @brief Print value
 *
 * Prints value for user (just in case we later make
 * objects of this kind available to the parser).
 *
 * @param o     The stream for printing
 *
 */
void FunctionDef::printValue(std::ostream &o) const {

    o << briefInfo() << std::endl;
}


