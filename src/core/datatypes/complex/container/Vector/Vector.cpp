/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Container
 *
 * $Id$
 */

#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"


/** Set type of elements */
Vector::Vector(const std::string& elemType)
    : Container(TypeSpec(elemType)) {
}

/** Assignment operator; make sure we get independent elements */
Vector& Vector::operator=( const Vector& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        Container::operator=( x );
        
        for ( std::vector<RbLanguageObject*>::iterator i = elements.begin(); i != elements.end(); i++ )
            delete ( *i );
        elements.clear();
        
        for ( std::vector<RbLanguageObject*>::const_iterator i = x.elements.begin(); i != x.elements.end(); i++ )
            elements.push_back( (*i)->clone() );
    }
    
    return ( *this );
}

/** Clear contents of value container and make length 0 in all dimensions */
void Vector::clear( void ) {
    
    for ( std::vector<RbLanguageObject*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
        delete ( *i );
    }
    elements.clear();
    
    length = 0;
}


/** Get class vector describing type of object */
const VectorString& Vector::getClass(void) const { 

    static VectorString rbClass = VectorString(Vector_name) + Container::getClass();
	return rbClass;
}

/** Get element */
RbLanguageObject* Vector::getElement(const size_t index) {
    
    return elements[index];
}


/** Print value for user */
void Vector::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<RbLanguageObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}

/** Pop element off of front of vector, updating length in process */
void Vector::pop_front(void) {

    delete elements.front();
    elements.erase(elements.begin());
    length--;
}


/** Pop element off of back of vector, updating length in process */
void Vector::pop_back(void) {

    delete elements.back();
    elements.pop_back();
    length--;
}


/** Resize vector */
void Vector::resize( size_t n ) {

    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );

    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
    length = elements.size();
}

/** Set element */
void Vector::setElement(const size_t index, RbLanguageObject *elem) {
    if (index >= length) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    elements.insert(elements.begin()+index, elem);
}

