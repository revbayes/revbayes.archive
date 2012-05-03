/**
 * @file
 * This file contains the implementation of Integer, which is
 * a RevBayes wrapper around a regular int.
 *
 * @brief Implementation of Integer
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


#include "RbBoolean.h"
#include "Integer.h"
#include "Natural.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"

#include <sstream>

/* Default constructor */
Integer::Integer(void) : RbLanguageObject(), value(0) {
    
}


/** Construct from bool */
Integer::Integer(const bool v) : RbLanguageObject() {
    
    if (v) value = 1;
    else value = 0;
}


/* Construct from int */
Integer::Integer(const int v) : RbLanguageObject(), value(v) {

}


/* Construct from unsigned int (ambiguous between int and bool otherwise) */
Integer::Integer(const unsigned int v) : RbLanguageObject(), value(v) {
    
}


/* Construct from size_t (ambiguous between int and size_t otherwise) */
Integer::Integer(const unsigned long v) : RbLanguageObject(), value( int(v) ) {
    
}


/** Overloaded equals operator */
bool Integer::operator==(const Integer &x) const {
    
    return value == x.value;
}


/** Overloaded not-equals operator */
bool Integer::operator!=(const Integer &x) const {
    
    return value != x.value;
}

/** Overloaded < operator. */
bool Integer::operator<(const Integer &x) const {
    return value < x.value;
}


/** Clone object */
Integer* Integer::clone(void) const {

	return  new Integer(*this);
}


/** Convert to type. The caller manages the returned object. */
RbObject* Integer::convertTo( const TypeSpec& type ) const {

    if ( type == RbBoolean::getClassTypeSpec() ) 
        return new RbBoolean( value == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value );

    if ( type == RbString::getClassTypeSpec() ) {
        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    if ( type == RealPos::getClassTypeSpec() && value > 0 )
        return new RealPos( value );

    if ( type == Natural::getClassTypeSpec() && value >= 0)
        return new Natural( value );

    return RbLanguageObject::convertTo( type );
}


/** Get class name of object */
const std::string& Integer::getClassName(void) { 
    
    static std::string rbClassName = "Integer";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Integer::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbLanguageObject::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Integer::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/* Get a pointer to the elementary value. */
void* Integer::getLeanValue(std::vector<size_t> &length) const {
    return static_cast<void*>( &const_cast<int&>( value ) );
}


/** Is convertible to language object of type? */
bool Integer::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RbBoolean::getClassTypeSpec())
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RbString::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() && value > 0 )
        return true;

    if ( type == Natural::getClassTypeSpec() && value >= 0 )
        return true;

    return RbLanguageObject::isConvertibleTo( type );
}


/** Print value for user */
void Integer::printValue(std::ostream &o) const {

    o << value;
}


void Integer::setLeanValue(const RbValue<void *> &val) {
    value = *static_cast<int *>( val.value );
}



////////////////////////////////// Global Integer operators ///////////////////////////////////


/**
 * This function performs unary plus on an integer,
 * which simply returns a copy of the integer.
 *
 * @brief unary operator+
 *
 * @param  A The integer operand
 * @return A copy of the operand
 */
Integer operator+(const Integer& A) {

	Integer B = A;
	return B;
}


/**
 * This function performs unary minus on an integer.
 *
 * @brief operator-
 *
 * @param A The integer operand
 * @return -A
 */
Integer operator-(const Integer& A) {

	Integer B( - A.getValue() );

    return B;
}


/**
 * This function performs unary not on an integer by
 * comparing the value to 0. It returns true if the
 * value is different from 0.
 *
 * @brief operator!
 *
 * @param A The integer operand
 * @return True if A != 0
 */
RbBoolean operator!(const Integer& A) {

	RbBoolean B( A.getValue() != 0 );

    return B;
}

