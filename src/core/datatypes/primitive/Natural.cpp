/**
 * @file
 * This file contains the implementation of Natural, which is
 * the primitive RevBayes type for natural numbers (including 0).
 *
 * @brief Implementation of Natural
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "RbBoolean.h"
#include "Natural.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include <climits>
#include <sstream>
#include <climits>



/** Default constructor */
Natural::Natural( void ) : Integer( 0 ) {
}


/** Construct from bool */
Natural::Natural( bool x ) : Integer() {

    if ( x )
        value = 1;
    else
        value = 0;
}


/** Construct from int */
Natural::Natural( int x ) : Integer() {

    if ( x < 0 )
        throw RbException( "Negative value for " + getClassName() );

    value = x;
}


/* Construct from unsigned int */
Natural::Natural( unsigned int x ) : Integer() {
        
    value = x;
}


/* Construct from unsigned long */
Natural::Natural( unsigned long x) : Integer() {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + getClassName() );

    value = int(x);
}


/** Construct from Integer */
Natural::Natural( const Integer& x ) : Integer() {

    if ( x.getValue() < 0 )
        throw RbException( "Negative value for " + getClassName() );

    value = x.getValue();
}


/** Construct from Bool */
Natural::Natural( const RbBoolean& x) : Integer() {

    if ( x )
        value = 1;
    else
        value = 0;
}


/** Clone object */
Natural* Natural::clone( void ) const {

	return  new Natural( *this );
}


/** Convert to type. The caller manages the returned object. */
RbObject* Natural::convertTo( const TypeSpec& type ) const {

    if ( type == RbBoolean::getClassTypeSpec() )
        return new RbBoolean( value == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value );

    if ( type == RealPos::getClassTypeSpec() )
        return new RealPos( value );

    if ( type == RbString::getClassTypeSpec() ) {

        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    return Integer::convertTo( type );
}


/** Get class name of object */
const std::string& Natural::getClassName(void) { 
    
    static std::string rbClassName = "Natural";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Natural::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Integer::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Natural::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Is convertible to type? */
bool Natural::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RbBoolean::getClassTypeSpec() )
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() )
        return true;

    if ( type == RbString::getClassTypeSpec() )
        return true;

    return Integer::isConvertibleTo( type );
}


/** Print value for user */
void Natural::printValue( std::ostream &o ) const {

    o << value;
}


/** Set value from int */
void Natural::setValue( int x ) {

    if ( x < 0 )
        throw RbException( "Negative value for " + getClassName() );

    value = x;
}


/* Set value from size_t */
void Natural::setValue( size_t x ) {
    
    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + getClassName() );
    
    value = (size_t)x;
}


////////////////////////////////// Global Natural operators ///////////////////////////////////


///**
// * This function performs plus on two naturals,
// * which returns another natural.
// *
// * @brief binary operator+
// *
// * @param  A The first operand
// * @param  B The second operand
// * @return The sum of both as a natural number
// */
//Natural operator+(const Natural& A, const Natural& B) {
//    
//	Natural C = Natural( A.getValue() + B.getValue() );
//	return C;
//}
//
///**
// * This function performs multiplication on two naturals,
// * which returns another natural.
// *
// * @brief binary operator*
// *
// * @param  A The first operand
// * @param  B The second operand
// * @return The product of both as a natural number
// */
//Natural operator*(const Natural& A, const Natural& B) {
//    
//	Natural C = Natural( A.getValue() * B.getValue() );
//	return C;
//}
//
///**
// * This function performs division on two naturals,
// * which returns another natural.
// *
// * @brief binary operator/
// *
// * @param  A The first operand
// * @param  B The second operand
// * @return The quotient of both as a positive real number
// */
//RealPos operator/(const Natural& A, const Natural& B) {
//    
//	RealPos C = RealPos( A.getValue() / double( B.getValue() ) );
//	return C;
//}
//
