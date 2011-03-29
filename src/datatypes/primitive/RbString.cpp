/**
 * @file
 * This file contains the implementation of RbString, which is
 * a RevBayes wrapper around a regular string.
 *
 * @brief Implementation of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */
 
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorString.h"


/** Constructor from string */
RbString::RbString(const std::string& v) : RbObject(), value(v) {
}


/** Constructor from int */
RbString::RbString(int i) : RbObject() {

    std::ostringstream o;
    o << i;
    value = o.str();
}


/** Constructor from string */
RbString::RbString(double x) : RbObject() {

    std::ostringstream o;
    o << x;
    value = o.str();
}


/** Type conversion to string */
RbString::operator std::string() const {

    return value;
}


/** String concatenation with operator+  */
RbString RbString::operator+(const RbString& s) const {

    return RbString(value+s.value);
}


/** Clone function */
RbString* RbString::clone() const {

	return new RbString(*this);
}


/** Get class vector describing type of object */
const VectorString& RbString::getClass() const {

    static VectorString rbClass = VectorString(RbString_name) + RbObject::getClass();
    return rbClass;
}


/** Print value */
void RbString::printValue(std::ostream& o) const {

	o << "\"" << value << "\"";
}


/** Complete info on object */
std::string RbString::richInfo() const {

	return "RbString; value = '" + value + "'";
}


/** Global operator for appending to std::string */
std::string operator+( const std::string& A, const RbString& B ) {

    return A + std::string( B );
}

