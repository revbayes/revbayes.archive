/**
 * @file
 * This file contains the implementation of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Implementation of Integer
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "Boolean.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorString.h"

#include <sstream>

/** Construct from int */
Integer::Integer(const int v)
    : RbObject(), value(v) {
}

/** Construct from bool */
Integer::Integer(const bool v)
    : RbObject() {

    if (v) value = 1;
    else value = 0;
}

/** Clone object */
RbObject* Integer::clone(void) const {

	return  (RbObject*)(new Integer(*this));
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Integer::convertTo(const std::string& type) const {

    if (type == RbBool_name) 
        return new Boolean(value == 0);
    else if (type == Real_name)
        return new Real(value);
    else if (type == RealPos_name && value > 0)
        return new RealPos(value);
    else if (type == RbString_name) 
		{
        std::ostringstream o;
        o << value;
        return new RbString(o.str());
		}
    else if (type == VectorInteger_name)
        return new VectorInteger(value);
    throw RbException("Cannot convert int to " + type + ".");
    return NULL;
}

/** Pointer-based equals comparison */
bool Integer::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const Integer* p = dynamic_cast<const Integer*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to an int
    p = dynamic_cast<const Integer*>(obj->convert(Integer_name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}

/** Get class vector describing type of object */
const VectorString& Integer::getClass() const {

    static VectorString rbClass = VectorString(Integer_name) + RbObject::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool Integer::isConvertibleTo(const std::string& type) const {

    if (type == RbBool_name)
        return true;
    else if (type == Real_name)
        return true;
    else if (type == RealPos_name && value > 0)
        return true;
    else if (type == RbString_name)
        return true;
    else if (type == VectorInteger_name)
        return true;
    return false;
}

/** Print value for user */
void Integer::printValue(std::ostream &o) const {

    o << value;
}

/** Get complete info about object */
std::string Integer::toString(void) const {

	std::ostringstream o;
    o << "Integer(";
    printValue(o);
	o<< ")";

    return o.str();
}

