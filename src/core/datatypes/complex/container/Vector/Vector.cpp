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
#include "RbUtil.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"
#include <algorithm>

/** Set type of elements */
Vector::Vector(void) : AbstractVector(TypeSpec(RbLanguageObject_name)), typeSpec(Vector_name, new TypeSpec( RbLanguageObject_name ) ) {
}

/** Set type of elements */
Vector::Vector(const TypeSpec& elemType) : AbstractVector(elemType), typeSpec(Vector_name, new TypeSpec(elemType) ) {
}


/** Copy Constructor */
Vector::Vector(const Vector &v) : AbstractVector(v), typeSpec(Vector_name, new TypeSpec(elementType) ) {
    
    // copy all the elements by deep copy
    for (std::vector<RbLanguageObject* >::const_iterator it=v.elements.begin(); it!=v.elements.end(); it++) {
        RbLanguageObject *copy = (*it)->clone();
        elements.push_back( copy );
    }
    
}


/** Destructor. Free the memory of the elements. */
Vector::~Vector(void) {
    
    // just call clear which will free the memory of the elements
    clear();
}

/** Assignment operator; make sure we get independent elements */
Vector& Vector::operator=( const Vector& x ) {
    
    if ( this != &x ) {
        
        // First assign using parent assignment operator. This will test to make sure the containers
        // are of the same type, and throw an error if they are not. By calling it before we destroy
        // our own elements, we can make sure that an assignment error leaves us intact, which it should
        AbstractVector::operator=( x );
        
        // just call clear which will free the memory of the objects
        clear();
        
        for ( std::vector<RbLanguageObject* >::const_iterator i = x.elements.begin(); i != x.elements.end(); i++ ) {
            RbLanguageObject *element = (*i)->clone();
            elements.push_back( element );
        }
    }
    
    return ( *this );
}

/** Subscript operator */
RbLanguageObject& Vector::operator[](size_t i) {
    
    if (i > elements.size())
        throw RbException("Index out of bounds");
    
    return *elements[i];
}


/** Subscript const operator */
const RbLanguageObject& Vector::operator[](size_t i) const {
    
    if ( i >= elements.size() )
        throw RbException("Index out of bounds");
    return *elements[i];
}


/** Clear contents of value container and make length 0 in all dimensions */
void Vector::clear( void ) {
    
    for (std::vector<RbLanguageObject*>::iterator it = elements.begin(); it != elements.end(); it++) {
        RbLanguageObject* theElement = *it;
        delete theElement;
    }
    
    elements.clear();
    
}


Vector* Vector::clone() const {
    return new Vector(*this);
}


/** Get class vector describing type of object */
const VectorString& Vector::getClass(void) const { 

    static VectorString rbClass = VectorString(Vector_name) + AbstractVector::getClass();
	return rbClass;
}


/** Get element */
const RbObject& Vector::getElement(size_t index) const {
    
    return *elements[index];
}


/** Get element */
RbObject& Vector::getElement(size_t index) {
    
    return *elements[index];
}


const std::vector<RbLanguageObject* >& Vector::getValue(void) const {
    return elements;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& Vector::getTypeSpec(void) const {
    return typeSpec;
}


/** Print value for user */
void Vector::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<RbLanguageObject* >::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
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

    elements.erase(elements.begin());
}


/** Pop element off of back of vector, updating length in process */
void Vector::pop_back(void) {
    
    elements.pop_back();
}


/** Push an int onto the back of the vector after checking */
void Vector::push_back( RbObject* x ) {
    if (x == NULL) {
        elements.push_back( NULL );
    }
    else if ( x->isTypeSpec( TypeSpec(RbLanguageObject_name) ) ) {
        elements.push_back( static_cast<RbLanguageObject*>( x ) );
    } else if ( x->isConvertibleTo(RbLanguageObject_name) ) {
        elements.push_back( static_cast<RbLanguageObject*>(x->convertTo(RbLanguageObject_name) ) );
        // since we own the parameters, we will delete them
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Vector_name + "[] with invalid value" );
    }
}


/** Push an int onto the front of the vector after checking */
void Vector::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( TypeSpec(RbLanguageObject_name) ) ) {
        elements.insert( elements.begin(), static_cast<RbLanguageObject*>( x ) );
    } else if ( x->isConvertibleTo(RbLanguageObject_name) ) {
        elements.insert( elements.begin(), static_cast<RbLanguageObject*>(x->convertTo(RbLanguageObject_name) ) );
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Vector_name + "[] with invalid value" );
    }
}


/** Resize vector */
void Vector::resize( size_t n ) {

    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink vector" );

    for ( size_t i = elements.size(); i < n; i++ )
        elements.push_back( NULL );
}


/** Set element */
void Vector::setElement(const size_t index, RbLanguageObject* elem) {
    if (index >= elements.size()) {
        throw RbException("Cannot set element in Vector outside the current range.");
    }
    
    // remove first the old element at the index
    elements.erase(elements.begin()+index);
    
    elements.insert(elements.begin()+index, elem);
}

/** Get the size of the vector */
size_t Vector::size( void ) const {
    
    return elements.size();

}


/* Sort the vector */
void Vector::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resize the vector */
void Vector::unique(void) {
    
    sort();
    std::vector<RbLanguageObject* > uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}


