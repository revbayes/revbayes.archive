/**
 * @file
 * This file contains the implementation of VectorRealPos, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of VectorRealPos
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "Simplex.h"
#include "VectorInteger.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <cmath>
#include <iomanip>
#include <sstream>



/** Construct vector with one double x */
VectorRealPos::VectorRealPos(const double x) : VectorReal(x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + VectorRealPos_name);
}


/** Construct vector with n doubles x */
VectorRealPos::VectorRealPos(const size_t n, const double x) : VectorReal(n, x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + VectorRealPos_name);
}


/** Constructor from double vector */
VectorRealPos::VectorRealPos(const std::vector<double>& x) : VectorReal(x) {

    for (size_t i=0; i<x.size(); i++)
        {
        if (x[i] <= 0.0)
            throw RbException("Nonpositive value for " + VectorRealPos_name);
        }
}


/** Clone function */
VectorRealPos* VectorRealPos::clone(void) const {

    return new VectorRealPos(*this);
}


/** Convert to object of another class. The caller manages the object. */
RbObject* VectorRealPos::convertTo(const std::string& type) const {

    throw RbException("Cannot convert VectorRealPos to " + type + ".");
	return NULL;
}


/** Pointer-based equals comparison */
bool VectorRealPos::equals(const RbObject* obj) const {

    // Check type first
    if ( !obj->isType( VectorRealPos_name ) )
        return false;

    // Then use base class comparison
    if ( !VectorReal::equals(obj) )
        return false;

    return true;
}


/** Get class vector describing type of object */
const VectorString& VectorRealPos::getClass(void) const {

    static VectorString rbClass = VectorString(VectorRealPos_name) + VectorReal::getClass();
    return rbClass;
}


/** Get element type */
const std::string& VectorRealPos::getElementType(void) const {

    static std::string rbType = RealPos_name;
    return rbType;
}


/** Convert to object of another class. The caller manages the object. */
bool VectorRealPos::isConvertibleTo(const std::string& type) const {

    return false;
}


/** Check value before pushing it back */
void VectorRealPos::push_back(double x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + VectorRealPos_name);

    VectorReal::push_back(x);
}


/** Check value before adding it in front */
void VectorRealPos::push_front(double x) {

    if (x <= 0.0)
        throw RbException("Nonpositive value for " + VectorRealPos_name);

    VectorReal::push_front(x);
}


/** Allow parser to set an element (any type conversion is done by parser) */
void VectorRealPos::setElement(const VectorInteger& index, RbObject* val) {

    if ( !val->isType(RealPos_name) )
        throw (RbException("Type mismatch"));

    if ( index.size() != 1 || index[0] < 1 )
        throw (RbException("Index error"));

    VectorReal::setElement(index, val);
}


/** Set the value directly */
void VectorRealPos::setValue(const std::vector<double>& x) {
    
    for (size_t i=0; i<x.size(); i++)
        if (value[i] <= 0.0)
            throw RbException("Trying to set positive real matrix with negative value(s)");
    
    VectorReal::setValue(x);
}


/** Complete info about object */
std::string VectorRealPos::toString(void) const {

    std::ostringstream o;
    o <<  "VectorRealPos: value = ";
    printValue(o);

    return o.str();
}

