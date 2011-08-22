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
#include "RbUtil.h"
#include "RbString.h"
#include "VectorString.h"


/** Constructor from string */
RbString::RbString(const std::string& v) : RbLanguageObject(), value(v) {
}


/** Constructor from int */
RbString::RbString(int i) : RbLanguageObject() {

    std::ostringstream o;
    o << i;
    value = o.str();
}


/** Constructor from string */
RbString::RbString(double x) : RbLanguageObject() {

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


/** String output with operator<<  */
std::ostream& operator<<(std::ostream& stream, const RbString& s) {

	stream << s.value;
	return stream;
}

/** Brief info on object */
std::string RbString::briefInfo() const {
    
	return "RbString; value = '" + value + "'";
}

/** Clone function */
RbString* RbString::clone() const {

	return new RbString(*this);
}


/** Get class vector describing type of object */
const VectorString& RbString::getClass() const {

    static VectorString rbClass = VectorString(RbString_name) + RbLanguageObject::getClass();
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


/** Operator == for testing equality */
RbBoolean       operator==(const RbString& A, const RbString& B){
    
    return std::string( A ) == std::string( B );
}

/** Operator != for testing inequality */
RbBoolean       operator!=(const RbString& A, const RbString& B){
    
    return std::string( A ) != std::string( B );
}
