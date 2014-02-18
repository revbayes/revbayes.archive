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


#include "ConstantNode.h"
#include "RlBoolean.h"
#include "Natural.h"
#include "Integer.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RlString.h"
#include "TypeSpec.h"
#include <climits>
#include <sstream>
#include <climits>

using namespace RevLanguage;

/** Default constructor */
Natural::Natural( void ) : Integer( 0 ) {
}


Natural::Natural( RevBayesCore::TypedDagNode<int> *v ) : Integer( v ) {
    
}


/** Construct from int */
Natural::Natural( int x ) : Integer( x ) {

    if ( x < 0 )
        throw RbException( "Negative value for " + getClassName() );
}


/* Construct from unsigned int */
Natural::Natural( unsigned int x ) : Integer( x ) {
        
}


/* Construct from unsigned long */
Natural::Natural( unsigned long x) : Integer( int(x) ) {

    if ( x > INT_MAX )
        throw RbException( "Value out of range for " + getClassName() );

}


RbLanguageObject* Natural::add( const RbLanguageObject& rhs ) const 
{
    
    if ( rhs.getTypeSpec() == Natural::getClassTypeSpec() )
        return add( static_cast<const Natural&>( rhs ) );
    
    return Integer::add( rhs );
}


Natural* Natural::add(const RevLanguage::Natural &rhs) const
{
    
    Natural *n = new Natural( value->getValue() + rhs.getValue() );
    
    return n;
}


/** Clone object */
Natural* Natural::clone( void ) const {

	return new Natural( *this );
}


/** Convert to type. The caller manages the returned object. */
RbLanguageObject* Natural::convertTo( const TypeSpec& type ) const {

    if ( type == RlBoolean::getClassTypeSpec() )
        return new RlBoolean( value->getValue() == 0 );

    if ( type == Real::getClassTypeSpec() )
        return new Real( value->getValue() );

    if ( type == RealPos::getClassTypeSpec() )
        return new RealPos( value->getValue() );

    if ( type == RlString::getClassTypeSpec() ) {

        std::ostringstream o;
        printValue( o );
        return new RlString( o.str() );
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

    if ( type == RlBoolean::getClassTypeSpec() )
        return true;

    if ( type == Real::getClassTypeSpec() )
        return true;

    if ( type == RealPos::getClassTypeSpec() )
        return true;

    if ( type == RlString::getClassTypeSpec() )
        return true;

    return Integer::isConvertibleTo( type );
}
