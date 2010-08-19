/**
 * @file
 * This file contains the implementation of RbDouble, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Implementation of RbDouble
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
#include "RbDouble.h"
#include "RbNames.h"
#include "StringVector.h"

#include <iomanip>
#include <sstream>


/** Default constructor */
RbDouble::RbDouble(void)
    : RbObject(), value(0.0) {
}


/** Construct from double */
RbDouble::RbDouble(const double v)
    : RbObject(), value(v) {
}


/** Construct from int */
RbDouble::RbDouble(const int v)
    : RbObject(), value(v) {
}


/** Construct from bool */
RbDouble::RbDouble(const bool v)
    : RbObject() {

    if (v) value = 1.0;
    else value = 0.0;
}


/** Clone object */
RbObject* RbDouble::clone(void) const {

	return  (RbObject*)(new RbDouble(*this));
}

/** Convert to object of another class. The caller manages the object. */
RbObject* RbDouble::convertTo(const std::string& type) const {

    if (type == RbBool_name)
        return new RbBool(value == 0.0);
	return NULL;
}

/** Pointer-based equals comparison */
bool RbDouble::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const RbDouble* p = dynamic_cast<const RbDouble*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to a double
    p = dynamic_cast<const RbDouble*>(obj->convert(RbDouble_name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}


/** Get class vector describing type of object */
const StringVector& RbDouble::getClass() const {

    static StringVector rbClass = StringVector(RbDouble_name) + RbObject::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool RbDouble::isConvertibleTo(const std::string& type) const {

    if (type == RbBool_name)
        return true;
	return false;
}

/** Print value for user */
void RbDouble::printValue(std::ostream &o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << value;

    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Get complete info about object */
std::string RbDouble::toString(void) const {

	std::ostringstream o;
    o << "Real(";
    printValue(o);
	o << ")";
    return o.str();
}

