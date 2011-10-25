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


/** Complete constructor */
TypeSpec::TypeSpec(const std::string &objType, RbPtr<TypeSpec> elemType) : baseType(objType) {
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
        elementType = RbPtr<TypeSpec>( new TypeSpec(*ts.elementType) );
    }
    else {
        elementType = RbPtr<TypeSpec>::getNullPtr();
    }
    
    type = baseType;
    
    // add the element type
    if (elementType != NULL) {
        type += "<" + elementType->toString() + ">";
    }
}


/** Assignment operator; make sure we get independent elements */
TypeSpec& TypeSpec::operator=( const TypeSpec& x ) {
    
    if ( this != &x ) {
        
        elementType = RbPtr<TypeSpec>( new TypeSpec(*x.elementType) );
    }
    
    return ( *this );
}


/** Equals comparison */
bool TypeSpec::operator==(const TypeSpec& x) const {

    if ( type == x.type && elementType == x.elementType)
        return true;
    else
        return false;
}


/** Type conversion operator to std::string */
TypeSpec::operator std::string( void ) const {

    return toString();
}


/** Convert to string */
const std::string& TypeSpec::toString(void) const {

    return type;
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const TypeSpec& x) {

    o << x.toString();
    return o;
}


/** String concatenation using operator + */
std::string operator+(const std::string& s, const TypeSpec& x) {

    return s + x.toString();
}

