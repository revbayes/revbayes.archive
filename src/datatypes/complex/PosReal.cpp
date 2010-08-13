/**
 * @file
 * This file contains the implementation of PosReal, which is
 * used to hold strictly positive scalar values.
 *
 * @brief Implementation of PosReal
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


#include "PosReal.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"

#include <sstream>


/** Construct from double */
PosReal::PosReal(const double v)
    : RbDouble(v) {

    if (v <= 0.0)
        throw RbException("Nonpositive value for " + PosReal_name);
}


/** Construct from int */
PosReal::PosReal(const int v)
    : RbDouble(v) {

    if (v <= 0)
        throw RbException("Nonpositive value for " + PosReal_name);
}


/** Clone object */
RbObject* PosReal::clone(void) const {

	return  (RbObject*)(new PosReal(*this));
}


/** Convert to object of another class. Do not convert to bool as in base class. */
RbObject* PosReal::convertTo(const std::string& type) const {

    return NULL;
}


/** Get class vector describing type of object */
const StringVector& PosReal::getClass() const {

    static StringVector rbClass = StringVector(PosReal_name) + RbDouble::getClass();
    return rbClass;
}


/** Set value */
void PosReal::setValue(double x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + PosReal_name);

    RbDouble::setValue(x);
}


/** Get complete info about object */
std::string PosReal::toString(void) const {

	std::ostringstream o;
    o << "PosReal: value = ";
    printValue(o);

    return o.str();
}

