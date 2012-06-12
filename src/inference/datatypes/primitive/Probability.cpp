/**
 * @file
 * This file contains the implementation of Probability, which
 * is the primitive RevBayes type for positive real numbers between 0 and 1.
 *
 *
 * @brief Implementation of Probability
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */


#include "Probability.h"
#include "RealPos.h"
#include "RbException.h"
#include "RbString.h"
#include "RbUtil.h"

#include <sstream>


/** Default constructor */
Probability::Probability( void ) : RealPos( 1.0 ) {
}


/** Construct from double */
Probability::Probability( double x ) : RealPos( x ) {
    
    if ( x < 0.0 || x > 1.0)
        throw RbException( "Creation of " + getClassName() + " with value x=" + x + " outside standard probabilities (0,1)");
}


/** Construct from int */
Probability::Probability( int x ) : RealPos( x ) {
    
    if ( x < 0 || x > 1)
        throw RbException( "Creation of " + getClassName() + " with value x=" + x + " outside standard probabilities (0,1)");
}


/** Construct from unsigned int */
Probability::Probability( unsigned int x ) : RealPos( x ) {
    
    if ( x > 1)
        throw RbException( "Creation of " + getClassName() + " with value x=" + int(x) + " outside standard probabilities (0,1)");
}


/** Clone object */
Probability* Probability::clone( void ) const {
    
	return  new Probability( *this );
}


/** Get class name of object */
const std::string& Probability::getClassName(void) { 
    
    static std::string rbClassName = "Probability";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Probability::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RealPos::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Probability::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set value */
void Probability::setValue( double x ) {
    
    if ( x < 0.0 || x > 1.0)
        throw RbException( "Creation of " + getClassName() + " with value x=" + x + " outside standard probabilities (0,1)");
    
    RealPos::setValue( x );
}

