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
 * $Id:$
 */


#include "Boolean.h"
#include "Natural.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorBoolean.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorRealPos.h"
#include "VectorReal.h"
#include "VectorString.h"

#include <sstream>


/** Default constructor */
Natural::Natural( void ) : Integer( 0 ) {
}


/** Construct from int */
Natural::Natural( int x ) : Integer() {

    if ( x < 0 )
        throw RbException( "Negative value for " + Natural_name );

    value = x;
}


/** Construct from unsigned int */
Natural::Natural( unsigned int x) : Integer() {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + Natural_name );

    value = x;
}


/** Construct from bool */
Natural::Natural( bool x ) : Integer() {

    if ( x )
        value = 1;
    else
        value = 0;
}


/** Construct from Integer */
Natural::Natural( const Integer& x ) : Integer() {

    if ( x.getValue() < 0 )
        throw RbException( "Negative value for " + Natural_name );

    value = x.getValue();
}


/** Construct from Bool */
Natural::Natural( const Boolean& x) : Integer() {

    if ( x )
        value = 1;
    else
        value = 0;
}


/** Clone object */
Natural* Natural::clone( void ) const {

	return  new Natural( *this );
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* Natural::convertTo( const std::string& type, size_t dim ) const {

    if ( type == Boolean_name && dim == 0 ) 
        return new Boolean( value == 0 );

    if ( type == Real_name && dim == 0 )
        return new Real( value );

    if ( type == RealPos_name && dim == 0 )
        return new RealPos( value );

    if ( type == RbString_name && dim == 0) {
        
        std::ostringstream o;
        printValue( o );
        return new RbString( o.str() );
    }

    if ( type == Natural_name && dim == 1 )
        return new VectorNatural( value );

    if ( type == Integer_name && dim == 1 )
        return new VectorInteger( value );

    if ( type == Boolean_name && dim == 1 ) 
        return new VectorBoolean( value == 0 );

    if ( type == Real_name && dim == 1 )
        return new VectorReal( value );

    if ( type == RealPos_name && dim == 1 )
        return new VectorRealPos( value );

    return Integer::convertTo( type, dim );
}


/** Get class vector describing type of object */
const VectorString& Natural::getClass() const {

    static VectorString rbClass = VectorString(Natural_name) + Integer::getClass();
    return rbClass;
}


/** Is convertible to type and dim? */
bool Natural::isConvertibleTo( const std::string& type, size_t dim, bool once ) const {

    if ( type == Boolean_name && dim == 0 ) 
        return true;

    if ( type == Real_name && dim == 0 )
        return true;

    if ( type == RealPos_name && dim == 0 )
        return true;

    if ( type == RbString_name && dim == 0 )
        return true;

    if ( type == Natural_name && dim == 1 )
        return true;

    if ( type == Integer_name && dim == 1 )
        return true;

    if ( type == Boolean_name && dim == 1 ) 
        return true;

    if ( type == Real_name && dim == 1 )
        return true;

    if ( type == RealPos_name && dim == 1 )
        return true;

    return Integer::isConvertibleTo( type, dim, once );
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

