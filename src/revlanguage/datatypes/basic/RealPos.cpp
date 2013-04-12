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
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;
    
/** Default constructor */
RealPos::RealPos( void ) : Real( 1.0 ) {
}


/** Construct from double */
RealPos::RealPos( RevBayesCore::TypedDagNode<double> *x ) : Real( x ) {
    
    if ( x->getValue() < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from double */
RealPos::RealPos( double x ) : Real( x ) {

    if ( x < 0.0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Construct from int */
RealPos::RealPos( int x ) : Real( x ) {

    if ( x < 0 )
        throw RbException( "Nonpositive value for " + getClassName() );
}


/** Clone object */
RealPos* RealPos::clone( void ) const {

	return  new RealPos( *this );
}


/** Get class name of object */
const std::string& RealPos::getClassName(void) { 
    
    static std::string rbClassName = "RealPos";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RealPos::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Real::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RealPos::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

