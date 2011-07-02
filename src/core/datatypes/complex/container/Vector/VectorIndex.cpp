/**
 * @file
 * This file contains the implementation of VectorIndex, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Implementation of VectorIndex
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
#include "TypeSpec.h"
#include "VectorIndex.h"
#include "VectorInteger.h"
#include "VectorString.h"


/** Construct empty vector */
VectorIndex::VectorIndex( void ) : VectorObject() {
}


/** Index operator */
const RbObject* VectorIndex::operator[]( size_t i ) const {
    
    if ( i >= elements.size() )
        throw RbException( "Index out of bounds" );
    
    return elements[i];
}


/** Clone function */
VectorIndex* VectorIndex::clone( void ) const {
    
    return new VectorIndex( *this );
}


/** Get class vector describing type of object */
const VectorString& VectorIndex::getClass( void ) const {
    
    static VectorString rbClass = VectorString( VectorIndex_name ) + VectorObject::getClass();
    return rbClass;
}


/** Get int value of element i, if any */
int VectorIndex::getInt( size_t i ) const {
    
    if ( i >= elements.size() )
        throw RbException( "Index out of bounds" );
    
    if ( !elements[i]->isType( Integer_name ) )
        throw RbException( "No int value of index" );
    
    return static_cast<Integer*>( elements[i] )->getValue();
}


/** Get string value of element i, if any */
std::string VectorIndex::getString( size_t i ) const {
    
    if ( i >= elements.size() )
        throw RbException( "Index out of bounds" );
    
    if ( !elements[i]->isType( RbString_name ) )
        throw RbException( "No int value of index" );
    
    return static_cast<RbString*>( elements[i] )->getValue();
}


/** Is element i a negative int, that is, is element i an empty index? */
bool VectorIndex::isEmpty( size_t i ) const {
    
    if ( i >= elements.size() )
        throw RbException( "Index out of bounds" );
    
    if ( !elements[i]->isType( Integer_name ) )
        return false;
    
    return static_cast<Integer*>( elements[i] )->getValue() < 0;
}


/** Push an object onto the back of the vector afer type checking */
void VectorIndex::push_back( RbObject* x ) {

    if ( x->getDim() != 0 || !( x->isType( RbString_name) || x->isType( Integer_name ) ) )
        throw RbException( "Invalid index of type " + x->getTypeSpec().toString() );

    VectorObject::push_back( x );
}


/** Push an int onto the back of the vector */
void VectorIndex::push_back( int x ) {
    
    VectorObject::push_back( new Integer( x ) );
}


/** Push an object onto the back of the vector afer type checking */
void VectorIndex::push_back( std::string& s ) {
    
    VectorObject::push_back( new RbString( s ) );
}


/** Complete info about object */
std::string VectorIndex::richInfo( void ) const {
    
    std::ostringstream o;
    o <<  "VectorIndex: value = ";
    printValue( o );
    
    return o.str();
}


/** Convert to element index string for use in parser */
std::string VectorIndex::toIndexString(void) const {
    
    std::ostringstream o;
    
    for ( size_t i = 0; i < elements.size(); i++ ) {

        o << "[";
        if ( elements[i]->isType( Integer_name ) )
            o << static_cast<Integer*>( elements[i] )->getValue() + 1;
        else
            o << elements[i];
        o << "]";
    }
    
    return o.str();
}
