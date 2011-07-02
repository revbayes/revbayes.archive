/**
 * @file
 * This file contains the implementation of VectorObject, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Implementation of VectorObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "ContainerIterator.h"
#include "Natural.h"
#include "RbException.h"
#include "RbNames.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorObject.h"
#include "VectorString.h"


/** Construct empty vector */
VectorObject::VectorObject( void ) : Vector( RbObject_name ) {
}


/** Index operator */
const RbObject* VectorObject::operator[]( size_t i ) const {
    
    if ( i >= elements.size() )
        throw RbException( "Index out of bounds" );
    
    return elements[i];
}


/** Clone function */
VectorObject* VectorObject::clone( void ) const {
    
    return new VectorObject( *this );
}


/** Get class vector describing type of object */
const VectorString& VectorObject::getClass( void ) const {
    
    static VectorString rbClass = VectorString( VectorObject_name ) + Vector::getClass();
    return rbClass;
}


/** Push an object onto the back of the vector */
void VectorObject::push_back( RbObject* x ) {

    if ( x->getDim() != 0 )
        throw RbException( "Invalid attempt to create vector of containers" );

    elements.push_back( x );
    length[0]++;
}


/** Complete info about object */
std::string VectorObject::richInfo( void ) const {
    
    std::ostringstream o;
    o <<  "VectorObject: value = ";
    printValue( o );
    
    return o.str();
}

