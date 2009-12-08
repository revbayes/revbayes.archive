/**
 * @file
 * This file contains the implementation of RbBool, which is
 * a RevBayes wrapper around a regular bool.
 *
 * @brief Implementation of RbBool
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
#include "RbInt.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Construct from bool */
RbBool::RbBool(const bool v)
    : RbPrimitive(), value(v) {
}


/** Clone object */
RbObject* RbBool::clone(void) const {

	return  (RbObject*)(new RbBool(*this));
}


/** Convert to object of another class. The caller manages the object */
RbObject* RbBool::convertTo(const std::string& type) const {

	if (type == RbNames::RbInt::name) {
    	if (value) return new RbInt(1);
        else return new RbInt(0);
    }
    else if (type == RbNames::RbDouble::name) {
        if (value) return new RbDouble(1.0);
        else return new RbDouble(0.0);
    }

    return NULL;
}


/** Pointer-based equals comparison */
bool RbBool::equals(const RbObject* obj) const {

    // Use built-in fast down-casting first
    const RbBool* p = dynamic_cast<const RbBool*>(obj);
    if (p != NULL)
        return value == p->value;

    // Try converting the object to a bool
    p = dynamic_cast<const RbBool*>(obj->convertTo(RbNames::RbBool::name));
    if (p == NULL)
        return false;

    // Get result
    bool result = (value == p->value);
    delete p;
    return result;
}


/** Get class vector describing type of object */
const StringVector& RbBool::getClass() const {

    static StringVector rbClass = StringVector(RbNames::RbBool::name) + RbPrimitive::getClass();
    return rbClass;
}


/** Print value for user */
void RbBool::printValue(std::ostream &o) const {

    o << (value ? "true" : "false");
}


/** Get complete info about object */
std::string RbBool::toString(void) const {

	std::ostringstream o;
    o << "RbBool: value = ";
    printValue(o);

    return o.str();
}

