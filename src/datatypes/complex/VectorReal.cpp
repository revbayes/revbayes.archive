/**
 * @file
 * This file contains the implementation of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of VectorReal
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
#include "VectorReal.h"
#include "VectorString.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>



/** Construct vector with one double x */
VectorReal::VectorReal(const double x) : RbComplex() {

    value.push_back(x);
}

/** Construct vector with n doubles x */
VectorReal::VectorReal(const size_t n, const double x) : RbComplex() {

    for (size_t i = 0; i < n; i++)
        value.push_back(x);
}

/** Constructor from double vector */
VectorReal::VectorReal(const std::vector<double>& x) : RbComplex() {

    value = x;
}

/** Clone function */
VectorReal* VectorReal::clone(void) const {

    return new VectorReal(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* VectorReal::convertTo(const std::string& type) const {

    throw RbException("Cannot convert VectorReal to " + type + ".");
	return NULL;
}


/** Pointer-based equals comparison */
bool VectorReal::equals(const RbObject* obj) const {

    // Check type first
    if ( !obj->isType( VectorReal_name ) )
        return false;

    // Now go through all elements
    const VectorReal& vec = *((VectorReal*)(obj));

    if ( vec.size() != value.size() )
        return false;

    for ( size_t i=0; i<vec.size(); i++)
        if ( vec[i] != value[i] )
            return false;
            
    return true;
}


/** Get class vector describing type of object */
const VectorString& VectorReal::getClass(void) const {

    static VectorString rbClass = VectorString(VectorReal_name) + RbComplex::getClass();
    return rbClass;
}


/** Get element for parser (read-only) */
const RbObject* VectorReal::getElement(const VectorInteger& index) const {

    static Real x = Real(0.0);

    if (index.size() != 1)
        throw (RbException("Index error"));
    if (index[0] >= (double)value.size() || index[0] < 0)
        throw (RbException("Index out of bound"));

    x.setValue(value[index[0]]);
    return &x;
}


/** Get element type */
const std::string& VectorReal::getElementType(void) const {

    static std::string rbType = Real_name;
    return rbType;
}


/** Get element length for parser */
const VectorInteger& VectorReal::getLength(void) const {

    static VectorInteger length = VectorInteger(0);

    length[0] = int(value.size());
    return length;
}

/** Convert to object of another class. The caller manages the object. */
bool VectorReal::isConvertibleTo(const std::string& type) const {

    return false;
}

/** Allow the parser to resize the vector */
void VectorReal::resize(const VectorInteger& len) {

    if (len.size() != 1 || len[0] < 0)
        throw (RbException("Length specification error"));

    value.resize(len[0]);
}


/** Allow parser to set an element (any type conversion is done by parser) */
void VectorReal::setElement(const VectorInteger& index, RbObject* val) {

    if ( !val->isType(Real_name) )
        throw (RbException("Type mismatch"));

    if ( index.size() != 1 || index[0] < 1 )
        throw (RbException("Index error"));

    // Do we want to allow resize to fit the new element or throw an error?
    // If we resize, then we have to fill in elements with some default value
    // or alternatively, keep a vector of bools signifying which elements should
    // be considered null elements. Here we use nan.
    if ( index[0] >= int(value.size()) ) {
        int oldLen = int(value.size());
        resize(index[0]);
        for (int i=oldLen; i<index[0]; i++)
            value[i] = std::numeric_limits<double>::quiet_NaN();
    }

    value[index[0]] = ((Real*)(val))->getValue();
}


/** Allow parser to rearrange the container (actually do not allow it) */
void VectorReal::setLength(const VectorInteger& len) {

    if ( len.size() != 1 && len[0] != int(value.size()) )
        throw (RbException("Length specification error"));
}


/** Set value of simplex using VectorReal */
void VectorReal::setValue(const VectorReal& x) {

    value.resize(x.size());
    for (size_t i=0; i<x.size(); i++)    
        value[i] = x[i];
}   


/** Print value for user */
void VectorReal::printValue(std::ostream& o) const {

    int previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<double>::const_iterator i = value.begin(); i!= value.end(); i++) {
        if (i != value.begin())
            o << ", ";
        o << (*i);
    }
    o <<  " ]";

    o.setf(previousFlags);
    o << std::setprecision(previousPrecision);
}


/** Complete info about object */
std::string VectorReal::toString(void) const {

    std::ostringstream o;
    o <<  "VectorReal: value = ";
    printValue(o);

    return o.str();
}

