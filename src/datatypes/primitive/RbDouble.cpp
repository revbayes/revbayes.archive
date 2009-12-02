/**
 * @file
 * This file contains the implementation of RbDouble, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Implementation of the class RbDouble
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
#include <string>

#include "RbBool.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbInt.h"
#include "StringVector.h"


const StringVector RbDouble::rbClass = StringVector("double") + RbObject::rbClass;


/**
 * @brief Constructor
 *
 * Creates an instance from a double.
 *
 * @param v     Value of the object
 *
 */
RbDouble::RbDouble(const double v)
    : RbPrimitive(), value(v) {
}

RbDouble::RbDouble(const RbDouble& v) : RbPrimitive(), value(v.value) {

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
RbObject* RbDouble::convertTo(const std::string& type) const {

    if (type == "bool") {
        return new RbBool(value==0.0);
    }
    else if (type == "int") {
        return new RbInt(int(value));   //TODO Do we want int truncation of doubles?
    }

    return NULL;
}

/**
 * @brief is convertible to
 *
 * This function checks if this data type can be converted into the given data type.
 *
 * @param dt         the data type we want to convert to
 * @returns          true, if it can be converted
 *
 */
bool RbDouble::isConvertibleTo(const std::string& type) const {

    if (type == "bool") {
        return true;
    } else if (type == "int") {
        return true;
    } else if (type == "char") {
        return true;
    }

    return false;
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
bool RbDouble::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
	const RbDouble* x = dynamic_cast<const RbDouble*>(obj);
    if (x != NULL)
        return value == x->value;

    // Try converting the value to a double
    x = dynamic_cast<const RbDouble*>(obj->convertTo("double"));
    if (x == NULL)
        return false;

    bool result = (value == x->value);
    delete x;
    return result;
}

/**
 * @brief complete info
 *
 * return complete object info.
 *
 * @return o     The string describing this object
 *
 */
std::string RbDouble::toString(void) const {

	char temp[30];
	sprintf(temp, "%1.6lf", value);
	std::string tempStr = temp;
    return "Value = " + tempStr;
}

RbObject& RbDouble::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbDouble& x = dynamic_cast<const RbDouble&> (obj);

        RbDouble& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbDouble& x = dynamic_cast<const RbDouble&> (*(obj.convertTo("double")));

            RbDouble& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to double");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbDouble& RbDouble::operator=(const RbDouble& ar) {

    value = ar.value;
    return (*this);
}

/**
 * @brief Prdouble value
 *
 * Prints value for user.
 *
 * @param o     The stream for printing
 *
 */
void RbDouble::printValue(std::ostream &o) const {

    o << value << std::endl;
}


bool RbDouble::lessThan(const RbObject* o) const {

	
}

RbObject* RbDouble::add(const RbObject* o) const {

}

RbObject* RbDouble::subtract(const RbObject* o) const {

}

RbObject* RbDouble::multiply(const RbObject* o) const {

}

RbObject* RbDouble::divide(const RbObject* o) const {

}

RbObject* RbDouble::raiseTo(const RbObject* o) const {

}

