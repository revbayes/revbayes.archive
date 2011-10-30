/**
 * @file
 * This file contains the implementation of VectorInteger, a complex type
 * used to hold int vectors.
 *
 * @brief Implementation of VectorInteger
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#include "Integer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VectorReal.h"
#include "VectorRealPos.h"

#include <algorithm>
#include <sstream>


// Definition of the static type spec member
const TypeSpec VectorInteger::typeSpec(VectorInteger_name);

/** Default constructor */
VectorInteger::VectorInteger(void) : AbstractVector(Integer_name) {
}


/** Construct vector with one int x */
VectorInteger::VectorInteger(int x) : AbstractVector(Integer_name) {
    
    elements.push_back( x );
}


/** Construct vector with n ints x */
VectorInteger::VectorInteger(size_t n, int x) : AbstractVector(Integer_name) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }
    
}


/** Constructor from int vector */
VectorInteger::VectorInteger(const std::vector<int>& x) : AbstractVector(Integer_name) {

    elements = x;
}


/** Constructor from VectorInteger */
VectorInteger::VectorInteger(const VectorNatural& x) : AbstractVector(Integer_name) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back( x[i] );
    }
}


/** Constructor from size_t vector */
VectorInteger::VectorInteger(const std::vector<size_t>& x) : AbstractVector(Integer_name) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back( int(x[i]) );
    }
}


/** Subscript operator */
int& VectorInteger::operator[](size_t i) {

    if (i > elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const int& VectorInteger::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorInteger::operator==(const VectorInteger& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorInteger::operator!=(const VectorInteger& x) const {

    return !operator==(x);
}


void VectorInteger::clear(void) {
    elements.clear();
}


/** Clone function */
VectorInteger* VectorInteger::clone() const {

    return new VectorInteger(*this);
}


/** Can we convert this vector into another object? */
RbObject* VectorInteger::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorRealPos(d);
    }
    else if (type == VectorNatural_name) {
        
        return new VectorNatural( getValue() );
    }
    else if (type == VectorReal_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            d.push_back(*it);
        }
        
        return new VectorRealPos(d);
    }
    
    return AbstractVector::convertTo(type);
}

/** Get class vector describing type of object */
const VectorString& VectorInteger::getClass() const {

    static VectorString rbClass = VectorString(VectorInteger_name) + AbstractVector::getClass();
    return rbClass;
}


RbPtr<const RbObject> VectorInteger::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<RbObject> n( new Integer(elements[index]) );
    
    return n;
}


RbPtr<RbObject> VectorInteger::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<RbObject> n( new Integer(elements[index]) );
    
    return n;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorInteger::getTypeSpec(void) const {
    return typeSpec;
}


/** Export value as STL vector */
std::vector<int> VectorInteger::getValue(void) const {

    return elements;
}


/** Can we convert this vector into another object? */
bool VectorInteger::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorNatural_name) {
        
        for (std::vector<int>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            
            // test whether we can convert this element, otherwise return false
            if (*it < 0) {
                return false;
            }
        }
        
        return true;
    }
    else if (type == VectorReal_name || type == VectorRealPos_name) {
        
        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorInteger::pop_back(void) {
    elements.pop_back();
}


void VectorInteger::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Push an int onto the back of the vector after checking */
void VectorInteger::push_back( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Integer_name) ) ) {
        elements.push_back(static_cast<Integer*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Integer_name) ) {
        elements.push_back(static_cast<Integer*>(x->convertTo(Integer_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Integer_name + "[] with invalid value" );
    }
}

/** Append element to end of vector, updating length in process */
void VectorInteger::push_back(int x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorInteger::push_front( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Integer_name) ) ) {
        elements.insert( elements.begin(), static_cast<Integer*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Integer_name) ) {
        elements.insert( elements.begin(), static_cast<Integer*>(x->convertTo(Integer_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Integer_name + "[] with invalid value" );
    }
}


/** Add element in front of vector, updating length in process */
void VectorInteger::push_front(int x) {
    
    elements.insert(elements.begin(), x);
}


void VectorInteger::resize(size_t n) {
    elements.resize(n);
}


/** Complete info about object */
std::string VectorInteger::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorInteger: value = ";
    printValue(o);

    return o.str();
}


void VectorInteger::setElement(const size_t index, RbPtr<RbLanguageObject> x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(Integer_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Integer*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Integer_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Integer*>(x->convertTo(Integer_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Integer_name + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorInteger::setValue(const std::vector<int>& x) {

    elements = x;
}   


/** Set value of vector using VectorInteger */
void VectorInteger::setValue(const VectorInteger& x) {

    elements = x.elements;
}   


size_t VectorInteger::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorInteger::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resizes the vector */
void VectorInteger::unique(void) {

    sort();
    std::vector<int> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i=1 ; i<elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}


