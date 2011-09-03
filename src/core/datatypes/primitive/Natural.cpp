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
#include "VectorBoolean.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorRealPos.h"
#include "VectorReal.h"
#include "VectorString.h"
#include <climits>
#include <sstream>
#include <climits>



// Definition of the static type spec member
const TypeSpec Natural::typeSpec(Natural_name);

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
        throw RbException( "Negative value for " + Natural_name );

    value = x;
}


/* Construct from unsigned int */
Natural::Natural( unsigned int x ) : Integer() {
        
    value = x;
}


/* Construct from unsigned long */
Natural::Natural( unsigned long x) : Integer() {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + Natural_name );

    value = int(x);
}


/** Construct from Integer */
Natural::Natural( const Integer& x ) : Integer() {

    if ( x.getValue() < 0 )
        throw RbException( "Negative value for " + Natural_name );

    value = x.getValue();
}


/** Construct from Bool */
Natural::Natural( const RbBoolean& x) : Integer() {

    if ( x )
        value = 1;
    else
        value = 0;
}


/** Get brief info about object */
std::string Natural::briefInfo( void ) const {

	std::ostringstream o;
    o << "Natural(";
    printValue( o );
	o << ")";

    return o.str();
}

/** Clone object */
Natural* Natural::clone( void ) const {

	return  new Natural( *this );
}


/** Convert to type. The caller manages the returned object. */
RbObject* Natural::convertTo( const TypeSpec& type ) const {

    if ( type == RbBoolean_name )
        return new RbBoolean( value == 0 );

    if ( type == Real_name )
        return new Real( value );

    if ( type == RealPos_name )
        return new RealPos( value );

    if ( type == RbString_name ) {

        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    if ( type == VectorNatural_name )
        return new VectorNatural( value );

    if ( type == VectorInteger_name )
        return new VectorInteger( value );

    if ( type == VectorBoolean_name )
        return new VectorBoolean( value == 0 );

    if ( type == VectorReal_name )
        return new VectorReal( value );

    if ( type == VectorRealPos_name )
        return new VectorRealPos( value );

    return Integer::convertTo( type );
}


/** Get class vector describing type of object */
const VectorString& Natural::getClass() const {

    static VectorString rbClass = VectorString(Natural_name) + Integer::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Natural::getTypeSpec(void) const {
    return typeSpec;
}


/** Is convertible to type and dim? */
bool Natural::isConvertibleTo( const TypeSpec& type ) const {

    if ( type == RbBoolean_name )
        return true;

    if ( type == Real_name )
        return true;

    if ( type == RealPos_name )
        return true;

    if ( type == RbString_name )
        return true;

    if ( type == VectorNatural_name )
        return true;

    if ( type == VectorInteger_name )
        return true;

    if ( type == VectorBoolean_name )
        return true;

    if ( type == VectorReal_name )
        return true;

    if ( type == VectorRealPos_name )
        return true;

    return Integer::isConvertibleTo( type );
}


/** Print value for user */
void Natural::printValue( std::ostream &o ) const {

    o << value;
}


/** Get complete info about object */
std::string Natural::richInfo( void ) const {

	std::ostringstream o;
    o << "Natural(";
    printValue( o );
	o << ")";

    return o.str();
}


/** Set value from int */
void Natural::setValue( int x ) {

    if ( x < 0 )
        throw RbException( "Negative value for " + Natural_name );

    value = x;
}


/** Set value from unsigned int */
void Natural::setValue( unsigned int x ) {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + Natural_name );

    value = x;
}

