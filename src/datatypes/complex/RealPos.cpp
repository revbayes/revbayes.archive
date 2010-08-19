/**
 * @file
 * This file contains the implementation of RealPos, which is
 * used to hold strictly positive scalar values.
 *
 * @brief Implementation of RealPos
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


#include "RealPos.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"

#include <sstream>



/** Construct from double */
RealPos::RealPos(const double v) : RbDouble(v) {

    if (v <= 0.0)
        throw RbException("Nonpositive value for " + RealPos_name);
}

/** Construct from int */
RealPos::RealPos(const int v) : RbDouble(v) {

    if (v <= 0)
        throw RbException("Nonpositive value for " + RealPos_name);
}

/** Clone object */
RbObject* RealPos::clone(void) const {

	return  (RbObject*)(new RealPos(*this));
}

/** Convert to object of another class. The caller manages the object. */
RbObject* RealPos::convertTo(const std::string& type) const {

    if (type == RbDouble_name)
        return new RbDouble(value);
    return NULL;}

/** Get class vector describing type of object */
const VectorString& RealPos::getClass() const {

    static VectorString rbClass = VectorString(RealPos_name) + RbDouble::getClass();
    return rbClass;
}

/** Convert to object of another class. The caller manages the object. */
bool RealPos::isConvertibleTo(const std::string& type) const {

    if (type == RealPos_name)
        return true;
    else if (type == RbDouble_name)
        return true;
    return false;
}

/** Set value */
void RealPos::setValue(double x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + RealPos_name);

    RbDouble::setValue(x);
}

/** Get complete info about object */
std::string RealPos::toString(void) const {

	std::ostringstream o;
    o << "+Real(";
    printValue(o);
	o << ")";

    return o.str();
}

