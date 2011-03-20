/**
 * @file
 * This file contains the implementation of Real, which is
 * a RevBayes wrapper around a regular double.
 *
 * @brief Implementation of Real
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id$
 */


#include "Boolean.h"
#include "Real.h"
#include "RealPos.h"
#include "RbNames.h"
#include "VectorString.h"

#include <iomanip>
#include <sstream>


/** Default constructor */
Real::Real(void) : RbObject(), value(0.0) {

}


/** Construct from double */
Real::Real(const double v)
    : RbObject(), value(v) {
}


/** Construct from int */
Real::Real(const int v)
    : RbObject(), value(v) {
}


/** Construct from bool */
Real::Real(const bool v)
    : RbObject() {

    if (v) value = 1.0;
    else value = 0.0;
}


/** Clone object */
RbObject* Real::clone(void) const {

	return  (RbObject*)(new Real(*this));
}


/** Convert to object of another class. The caller manages the object. */
RbObject* Real::convertTo(const std::string& type, int dim) const {

    if (type == Boolean_name && dim == 0)
        return new Boolean(value == 0.0);
    if (type == RealPos_name && dim == 0 && value > 0.0)
        return new RealPos(value);

    return RbObject::convertTo(type, dim);
}


/** Pointer-based equals comparison */
bool Real::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const Real* p = dynamic_cast<const Real*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to a double
    p = dynamic_cast<const Real*>(obj->convertTo(Real_name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}


/** Get class vector describing type of object */
const VectorString& Real::getClass() const {

    static VectorString rbClass = VectorString(Real_name) + RbObject::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool Real::isConvertibleTo(const std::string& type, int dim, bool once) const {

    if (type == Boolean_name && dim == 0)
        return true;
    if (type == RealPos_name && dim == 0 && once == true && value > 0.0)
        return true;

    return RbObject::isConvertibleTo(type, dim, once);
}


/** Print value for user */
void Real::printValue(std::ostream &o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << value;

    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Get complete info about object */
std::string Real::richInfo(void) const {

	std::ostringstream o;
    o << "Real(";
    printValue(o);
	o << ")";
    return o.str();
}

