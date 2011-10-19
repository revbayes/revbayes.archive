/**
 * @file
 * This file contains the implementation of RealPos, which
 * is the primitive RevBayes type for positive real numbers.
 *
 *
 * @brief Implementation of RealPos
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "RealPos.h"
#include "RbException.h"
#include "RbUtil.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec RealPos::typeSpec(RealPos_name);

/** Default constructor */
RealPos::RealPos( void ) : Real( 1.0 ) {
}


/** Construct from double */
RealPos::RealPos( double x ) : Real( x ) {

    if ( x < 0.0 )
        throw RbException( "Nonpositive value for " + RealPos_name );
}


/** Construct from int */
RealPos::RealPos( int x ) : Real( x ) {

    if ( x < 0 )
        throw RbException( "Nonpositive value for " + RealPos_name );
}


/** Construct from unsigned int */
RealPos::RealPos( unsigned int x ) : Real( x ) {
}


/** Get brief info about object */
std::string RealPos::briefInfo( void ) const {
    
	std::ostringstream o;
    o << "+Real(";
    printValue( o );
	o << ")";
    
    return o.str();
}

/** Clone object */
RealPos* RealPos::clone( void ) const {

	return  new RealPos( *this );
}


/** Get class vector describing type of object */
const VectorString& RealPos::getClass() const {

    static VectorString rbClass = VectorString( RealPos_name ) + Real::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& RealPos::getTypeSpec(void) const {
    return typeSpec;
}


/** Get complete info about object */
std::string RealPos::richInfo( void ) const {

	std::ostringstream o;
    o << "+Real(";
    printValue( o );
	o << ")";

    return o.str();
}


/** Set value */
void RealPos::setValue( double x ) {

    if ( x <= 0.0 )
        throw RbException( "Nonpositive value for " + RealPos_name );

    Real::setValue( x );
}

