/**
 * @file
 * This file contains the implementation of VectorNatural, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Implementation of VectorNatural
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#include "Natural.h"
#include "RbException.h"
#include "RbUtil.h"
#include "Real.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <algorithm>


// Definition of the static type spec member
const TypeSpec VectorNatural::typeSpec(VectorNatural_name);

/** Construct empty vector */
VectorNatural::VectorNatural( void ) : AbstractVector( Natural_name ) {
}


/** Construct vector with one natural number x from an int */
VectorNatural::VectorNatural( int x ) : AbstractVector( Natural_name ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );
    
    elements.push_back( x );
    
}


/** Construct vector with one natural number x from an unsigned int */
VectorNatural::VectorNatural( unsigned int x ) : AbstractVector( Natural_name ) {
    
    elements.push_back( x );
}


/** Construct vector with n copies of natural number x (int) */
VectorNatural::VectorNatural( size_t n, int x ) : AbstractVector( Natural_name ) {

    if ( x < 0 )
        throw RbException( "Trying to set " + Natural_name + "[] with negative value" );

    for ( size_t i = 0; i < n; i++ ) {
        elements.push_back( x );
    }
}


/** Construct vector with n copies of natural number x (unsigned int) */
VectorNatural::VectorNatural( size_t n, unsigned int x ) : AbstractVector( Natural_name ) {

    // No test needed, since unsigned ints are always positive
    // Natural will throw an error if the value is out of range
    for ( size_t i = 0; i < n; i++ ) {
        elements.push_back( x );
    }
}


/** Constructor from int vector */
VectorNatural::VectorNatural( const std::vector<int>& x ) : AbstractVector( Natural_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] < 0 )
            throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ ) {
        elements.push_back( x[i] );
    }
}


/** Constructor from unsigned int vector */
VectorNatural::VectorNatural( const std::vector<unsigned int>& x ) : AbstractVector( Natural_name ) {

    // No test needed, since unsigned ints are always positive
    // Natural will throw an error if the value is out of range
    elements = x;
}


/** Constructor from Integer vector (VectorInteger) */
VectorNatural::VectorNatural( const VectorInteger& x ) : AbstractVector( Natural_name ) {

    for ( size_t i = 0; i < x.size(); i++ ) {
        if ( x[i] < 0 )
            throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
    }

    for ( size_t i = 0; i < x.size(); i++ ) {
        elements.push_back( x[i] );
    }

}


/**
 * Subscript operator. By only implementing a value
 * subscript operator, we can prevent users of the class from
 * modifying a value without passing through the validation
 * checks in this class.
 */
unsigned int VectorNatural::operator[]( size_t i ) const {

    if ( i >= elements.size() )
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Equals comparison */
bool VectorNatural::operator==( const VectorNatural& x ) const {

    if ( size() != x.size() )
        return false;

    for ( size_t i = 0; i < elements.size(); i++) {
        if ( operator[](i) != x[i] )
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorNatural::operator!=( const VectorNatural& x ) const {

    return !operator==( x );
}


void VectorNatural::clear(void) {
    elements.clear();
}


/** Clone function */
VectorNatural* VectorNatural::clone( void ) const {

    return new VectorNatural( *this );
}


/** Can we convert this vector into another object? */
RbObject* VectorNatural::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<unsigned int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorRealPos(d);
    }
    else if (type == VectorInteger_name) {
        
        return new VectorInteger( getValue() );
    }
    else if (type == VectorReal_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<unsigned int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorReal(d);
    }
    
    return AbstractVector::convertTo(type);
}


/** Get class vector describing type of object */
const VectorString& VectorNatural::getClass( void ) const {

    static VectorString rbClass = VectorString( VectorNatural_name ) + AbstractVector::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorNatural::getTypeSpec(void) const {
    return typeSpec;
}


/** Get value as an STL vector of unsigned int */
std::vector<unsigned int> VectorNatural::getValue( void ) const {

    return elements;
}


const RbObject* VectorNatural::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    const RbObject* n = new Natural(elements[index]);
    
    return n;
}


RbObject* VectorNatural::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbObject* n = new Natural(elements[index]);
    
    return n;
}


/** Can we convert this vector into another object? */
bool VectorNatural::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name || type == VectorInteger_name || type == VectorReal_name) {
        
        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorNatural::pop_back(void) {
    elements.pop_back();
}


void VectorNatural::pop_front(void) {
    
    // free the memory
    elements.erase(elements.begin());
}


/** Push an int onto the back of the vector after checking */
void VectorNatural::push_back( RbObject* x ) {
    
    if ( x->isTypeSpec( TypeSpec(Natural_name) ) ) {
        elements.push_back(static_cast<Natural*>( x )->getValue());
    } else if ( x->isConvertibleTo(Natural_name) ) {
        elements.push_back(static_cast<Natural*>(x->convertTo(Natural_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Natural_name + "[] with invalid value" );
    }
}


/** Push an int onto the back of the vector after checking */
void VectorNatural::push_back( unsigned int x ) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorNatural::push_front( RbObject* x ) {
    
    if ( x->isTypeSpec( TypeSpec(Natural_name) ) ) {
        elements.insert( elements.begin(), static_cast<Natural*>( x )->getValue());
    } else if ( x->isConvertibleTo(Natural_name) ) {
        elements.insert( elements.begin(), static_cast<Natural*>(x->convertTo(Natural_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Natural_name + "[] with invalid value" );
    }
}


/** Push an int onto the front of the vector after checking */
void VectorNatural::push_front( unsigned int x ) {

    elements.insert(elements.begin(), x);
}


void VectorNatural::resize(size_t n) {
    elements.resize(n);
}


/** Complete info about object */
std::string VectorNatural::richInfo( void ) const {

    std::ostringstream o;
    o <<  "VectorNatural: value = ";
    printValue( o );

    return o.str();
}


void VectorNatural::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(Natural_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Natural*>( x )->getValue());
    } else if ( x->isConvertibleTo(Natural_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Natural*>(x->convertTo(Natural_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + Natural_name + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector of int */
void VectorNatural::setValue( const std::vector<int>& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            
            elements.push_back( x[i] );
        }
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            elements[i] = x[i];
        }
    }
}


/** Set value of vector using STL vector of unsigned int */
void VectorNatural::setValue( const std::vector<unsigned int>& x ) {

    elements = x;
}


/** Set value of vector using VectorNatural */
void VectorNatural::setValue( const VectorNatural& x ) {

    elements = x.elements;
}


/** Set value of vector using VectorInteger */
void VectorNatural::setValue( const VectorInteger& x ) {

    if ( x.size() != elements.size() ) {
    
        clear();
        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            elements.push_back( x[i] );
        }
    }
    else {

        for ( size_t i = 0; i < x.size(); i++ ) {
            if ( x[i] < 0 )
                throw RbException( "Trying to set " + Natural_name + "[] with negative value(s)" );
            elements[i] = x[i]; 
        }
    }
}


size_t VectorNatural::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorNatural::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resize the vector */
void VectorNatural::unique(void) {
    
    sort();
    std::vector<unsigned int> uniqueVector;
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

