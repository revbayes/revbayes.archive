/**
 * @file
 * This file contains the implementation of TypeSpec, which
 * is used to hold type specifications.
 *
 * @brief Implementation of TypeSpec
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Simple constructor */
TypeSpec::TypeSpec(const std::string &objType) : 
baseType( objType ), 
elementType( NULL ),
parent( NULL )
{
    
    type = baseType;
}


/** Complete constructor */
TypeSpec::TypeSpec(const TypeSpec& base, TypeSpec* elemType) : 
baseType( base.getBaseType() ), 
elementType( elemType ), 
parent( new TypeSpec( *base.getParentType() )  )
{
    
    type = baseType;
    
    // add the element type
    if (elementType != NULL) 
    {
        type += "<" + elementType->toString() + ">";
    }
}


/** Complete constructor */
TypeSpec::TypeSpec(const std::string &objType, TypeSpec* p, TypeSpec* elemType) : 
baseType( objType ), 
parent( p )
{
    elementType = elemType;
    
    type = baseType;
    
    // add the element type
    if (elementType != NULL) {
        type += "<" + elementType->toString() + ">";
    }
}

/** Copy constructor */
TypeSpec::TypeSpec(const TypeSpec& ts) : baseType(ts.baseType) {
    
    // make a independent copy of the element type
    if (ts.elementType != NULL) {
        elementType = new TypeSpec(*ts.elementType);
    }
    else {
        elementType = NULL;
    }
    
    // make a independent copy of the parent type
    if (ts.parent != NULL) {
        parent = new TypeSpec(*ts.parent);
    }
    else {
        parent = NULL;
    }
    
    type = baseType;
    
    // add the element type
    if (elementType != NULL) {
        type += "<" + elementType->toString() + ">";
    }
}

TypeSpec::~TypeSpec(void) {
    
    if (elementType != NULL)
        delete elementType;
    
    if (parent != NULL)
        delete parent;
}


/** Assignment operator; make sure we get independent elements */
TypeSpec& TypeSpec::operator=( const TypeSpec& x ) {
    
    if ( this != &x ) {
        
        baseType = x.baseType;
        type = x.type;
        if (x.elementType != NULL) {
            elementType = new TypeSpec(*x.elementType);
        }
        else {
            elementType = NULL;
        }
        if (x.parent != NULL) {
            parent = new TypeSpec(*x.parent);
        }
        else {
            parent = NULL;
        }
        
    }
    
    return ( *this );
}


/** Equals comparison */
bool TypeSpec::operator==(const TypeSpec& x) const {
    
    if (baseType != x.baseType) 
    {
        return false;
    }
    
    if ( elementType != NULL) 
    {
        
        if ( x.elementType != NULL ) 
        {
            return *elementType == *x.elementType;
        }
        else 
        {
            return false;
        }
        
    }
    else 
    {
        if ( x.elementType != NULL ) 
        {
            return false;
        }
    }

    return true;
}


/** Type conversion operator to std::string */
TypeSpec::operator std::string( void ) const {

    return toString();
}


/** Test whether the represent type is either the same or a derived type of the parameter. */
bool TypeSpec::isDerivedOf(const TypeSpec &x) const {
    
    // first we test if this type is the same as the argument type
    if ( operator==( x ) ) {
        return true;
    }
    
    // then we test if we are a specialization of type x
    if ( baseType == x.baseType && ( x.elementType == NULL || ( elementType != NULL && elementType->isDerivedOf(*x.elementType) ) ) ) {
        return true;
    }
    
    // now we climb up our derivation tree
    if ( parent != NULL ) {
        return parent->isDerivedOf(x);
    }
    
    return false;
}



/*
 * Set the element type for this type spec.
 */
void TypeSpec::setElementType(TypeSpec *et) {
    elementType = et;
}


/** Convert to RlString */
const std::string& TypeSpec::toString(void) const {

    return type;
}


/** Make sure we can print to stream using << operator */
std::ostream& RevLanguage::operator<<(std::ostream& o, const TypeSpec& x) {

    o << x.toString();
    return o;
}


/** RlString concatenation using operator + */
std::string RevLanguage::operator+(const std::string& s, const TypeSpec& x) {

    return s + x.toString();
}

