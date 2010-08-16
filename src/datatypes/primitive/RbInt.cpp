/**
 * @file
 * This file contains the implementation of RbInt, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Implementation of RbInt
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


#include "RbBool.h"
#include "PosReal.h"
#include "RbDouble.h"
#include "RbException.h"
#include "RbInt.h"
#include "RbNames.h"
#include "RbString.h"
#include "StringVector.h"

#include <sstream>

/** Construct from int */
RbInt::RbInt(const int v)
    : RbObject(), value(v) {
}

/** Construct from bool */
RbInt::RbInt(const bool v)
    : RbObject() {

    if (v) value = 1;
    else value = 0;
}

/** Clone object */
RbObject* RbInt::clone(void) const {

	return  (RbObject*)(new RbInt(*this));
}

/** Convert to object of another class. The caller manages the object. */
RbObject* RbInt::convertTo(const std::string& type) const {

    if (type == RbBool_name) 
        return new RbBool(value == 0);
    else if (type == RbDouble_name)
        return new RbDouble(value);
    else if (type == PosReal_name && value > 0)
        return new PosReal(value);
    else if (type == RbString_name) 
		{
        std::ostringstream o;
        o << value;
        return new RbString(o.str());
		}
    else if (type == IntVector_name)
        return new IntVector(value);
    throw RbException("Cannot convert int to " + type + ".");
    return NULL;
}

/** Pointer-based equals comparison */
bool RbInt::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const RbInt* p = dynamic_cast<const RbInt*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to an int
    p = dynamic_cast<const RbInt*>(obj->convert(RbInt_name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}

/** Get class vector describing type of object */
const StringVector& RbInt::getClass() const {

    static StringVector rbClass = StringVector(RbInt_name) + RbObject::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool RbInt::isConvertibleTo(const std::string& type) const {

    if (type == RbBool_name)
        return true;
    else if (type == RbDouble_name)
        return true;
    else if (type == PosReal_name && value > 0)
        return true;
    else if (type == RbString_name)
        return true;
    else if (type == IntVector_name)
        return true;
    return false;
}

/** Print value for user */
void RbInt::printValue(std::ostream &o) const {

    o << value;
}

/** Get complete info about object */
std::string RbInt::toString(void) const {

	std::ostringstream o;
    o << "Integer(";
    printValue(o);
	o<< ")";

    return o.str();
}

