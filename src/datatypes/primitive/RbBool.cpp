/**
 * @file
 * This file contains the implementation of RbBool, which is
 * a RevBayes wrapper around a regular bool.
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


//static StringVector RbBool::rbClass = StringVector("bool") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from a bool.
 *
 * @param v     Value of the object
 *
 */
RbBool::RbBool(const bool v)
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
RbObject* RbBool::convertTo(const std::string& type) const {

    if (type == "int") {
        if (value) return new RbInt(1);
        else return new RbInt(0);
    }
    else if (type == "double") {
        if (value) return new RbDouble(0.0);
        else return new RbDouble (1.0);
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
bool RbBool::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const RbBool* x = dynamic_cast<const RbBool*>(obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to a bool
    x = dynamic_cast<const RbBool*>(obj->convertTo("bool"));
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
void RbBool::print(std::ostream &o) const {

	RbObject::print(o);
    o << "Value = " << (value ? "true" : "false") << std::endl;
}


/**
 * @brief Print value
 *
 * Prints value for user.
 *
 * @param o     The stream for printing
 *
 */
void RbBool::printValue(std::ostream &o) const {

    o << (value ? "true" : "false") << std::endl;
}

