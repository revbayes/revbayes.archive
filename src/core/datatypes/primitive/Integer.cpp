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
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>

// Definition of the static type spec member
const TypeSpec Integer::typeSpec(Integer_name);

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


/** Get brief info about object */
std::string Integer::briefInfo(void) const {
    
	std::ostringstream o;
    o << "Integer(";
    printValue(o);
	o<< ")";
    
    return o.str();
}

/** Clone object */
Integer* Integer::clone(void) const {

	return  new Integer(*this);
}


/** Convert to type. The caller manages the returned object. */
RbObject* Integer::convertTo( const TypeSpec& type ) const {

    if ( type == RbBoolean_name ) 
        return new RbBoolean( value == 0 );

    if ( type == Real_name )
        return new Real( value );

    if ( type == RbString_name ) {
        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    if ( type == VectorInteger_name )
        return new VectorInteger( value );

    if ( type == RealPos_name && value > 0 )
        return new RealPos( value );

    if ( type == Natural_name && value >= 0)
        return new Natural( value );

    return RbLanguageObject::convertTo( type );
}


/** Get class vector describing type of object */
const VectorString& Integer::getClass() const {

    static VectorString rbClass = VectorString(Integer_name) + RbLanguageObject::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Integer::getTypeSpec(void) const {
    return typeSpec;
}


/** Is convertible to language object of type? */
bool Integer::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RbBoolean_name)
        return true;

    if ( type == Real_name )
        return true;

    if ( type == RbString_name )
        return true;
    
    if ( type == VectorInteger_name )
        return true;

//    if ( type == RealPos_name && value > 0 )
//        return true;
//
//    if ( type == Natural_name && value >= 0 )
//        return true;

    return RbLanguageObject::isConvertibleTo( type );
}


/** Print value for user */
void Integer::printValue(std::ostream &o) const {

    o << value;
}


/** Get complete info about object */
std::string Integer::richInfo(void) const {

	std::ostringstream o;
    o << "Integer(";
    printValue(o);
	o<< ")";

    return o.str();
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

