/**
 * @file
 * This file contains the implementation of RbInt, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Implementation of the class RbInt
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

#include "RbBool.h"
#include "RbDouble.h"
#include "RbInt.h"


//static StringVector RbInt::rbClass = StringVector("int") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from an int.
 *
 * @param v     Value of the object
 *
 */
RbInt::RbInt(const int v)
    : RbObject(), value(v) {
}


/**
 * @brief Conversion
 *
 * Converts object to another class. Returns NULL
 * on failure.
 *
 * @param type      The desired type
 * @returns         Pointer to a new object of the desired 
 *                  type or NULL if the conversion fails
 *
 */
RbObject* RbInt::convertTo(const std::string& type) const {

    if (type == "bool") {
        return new RbBool(value==0);
    }
    else if (type == "double") {
        return new RbDouble(value);
    }

    return NULL;
}


/**
 * @brief Pointer-based equal comparison
 *
 * Compares equality of this object to another RbObject.
 *
 * @param obj   The object of the comparison
 * @returns     Result of comparison
 *
 */
bool RbInt::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const RbInt* x = dynamic_cast<const RbInt*>(obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to an int
    x = dynamic_cast<const RbInt*>(obj->convertTo("int"));
    if (x == NULL)
        return false;

    bool result = (value == x->value);
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
void RbInt::print(std::ostream &o) const {

	RbObject::print(o);
    o << "Value = " << value << std::endl;
}


/**
 * @brief Print value
 *
 * Prints value for user.
 *
 * @param o     The stream for printing
 *
 */
void RbInt::printValue(std::ostream &o) const {

    o << value << std::endl;
}

