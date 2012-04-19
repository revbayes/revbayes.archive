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


/** Constructor from string */
RbString::RbString(const std::string& v) : RbLanguageObject(), value(v) {
}


/** Constructor from int */
RbString::RbString(int i) : RbLanguageObject() {

    std::ostringstream o;
    o << i;
    value = o.str();
}


/** Constructor from long */
RbString::RbString(long i) : RbLanguageObject() {
    
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


/** Overloaded equals operator */
bool RbString::operator==(const RbString &x) const {
    
    return value == x.value;
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


/** Overloaded < operator. */
//bool RbString::operator<(const RbString &x) const {
//    return value.compare( x.value );
//}

/** Clone function */
RbString* RbString::clone() const {

	return new RbString(*this);
}


/** Get class name of object */
const std::string& RbString::getClassName(void) { 
    
    static std::string rbClassName = "String";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbString::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RbString::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Get a pointer to the elementary value. */
void* RbString::getValue(std::vector<size_t> &length) const {
    return static_cast<void*>( &const_cast<std::string&>( value ) );
}


/** Print value */
void RbString::printValue(std::ostream& o) const {

//	o << "\"" << value << "\"";
	o << value;
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

RbBoolean       operator< (const RbString& A, const RbString& B){
    
    return std::string ( A ) < std::string ( B );
    
}

