/**
 * @file
 * This file contains the implementation of VectorBoolean, a complex type
 * used to hold vectors of RbBoolean values.
 *
 * @brief Implementation of VectorBoolean
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#include "RbBoolean.h"
#include "VectorBoolean.h"
#include "RbException.h"
#include "Integer.h"
#include "RbUtil.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>
#include <algorithm>

// Definition of the static type spec member
const TypeSpec VectorBoolean::typeSpec(VectorBoolean_name);

/** Default constructor */
VectorBoolean::VectorBoolean(void) : AbstractVector(RbBoolean_name) {

}


/** Construct vector with one bool x */
VectorBoolean::VectorBoolean(bool x) : AbstractVector(RbBoolean_name) {

    elements.push_back( x );
}


/** Construct vector with n bools x */
VectorBoolean::VectorBoolean(size_t n, bool x) : AbstractVector(RbBoolean_name) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }
}


/** Constructor from bool vector */
VectorBoolean::VectorBoolean(const std::vector<bool>& x) : AbstractVector(RbBoolean_name) {

    elements = x;
}


/** Constructor from int vector */
VectorBoolean::VectorBoolean(const std::vector<int>& x) : AbstractVector(RbBoolean_name) {

    for (std::vector<int>::const_iterator i=x.begin(); i!=x.end(); i++) {
        elements.push_back( (*i) == 0 );
    }
}

/** Subscript operator */
std::vector<bool>::reference VectorBoolean::operator[](size_t i) {

    if (i > elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
std::vector<bool>::const_reference VectorBoolean::operator[](size_t i) const {

    if ( i >= elements.size() )
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorBoolean::operator==(const VectorBoolean& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorBoolean::operator!=(const VectorBoolean& x) const {

    return !operator==(x);
}


void VectorBoolean::clear(void) {
    elements.clear();
}


/** Clone function */
VectorBoolean* VectorBoolean::clone() const {

    return new VectorBoolean(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorBoolean::getClass() const {

    static VectorString rbClass = VectorString(VectorBoolean_name) + AbstractVector::getClass();
    return rbClass;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorBoolean::getTypeSpec(void) const {
    return typeSpec;
}


const RbObject& VectorBoolean::getElement(size_t index) const {
    
    if ( index >= elements.size() )
        throw RbException("Index out of bounds");
    
    const RbObject* b = new RbBoolean(elements[index]);
    return *b;
}


RbObject& VectorBoolean::getElement(size_t index) {
    
    if ( index >= elements.size() )
        throw RbException("Index out of bounds");
    
    RbObject* b = new RbBoolean(elements[index]);
    return *b;
}


/** Export value as STL vector */
std::vector<bool> VectorBoolean::getValue(void) const {

    return elements;
}


void VectorBoolean::pop_back() {
    elements.pop_back();
}


void VectorBoolean::pop_front() {
    elements.erase(elements.begin());
}


/** Append element to end of vector, updating length in process */
void VectorBoolean::push_back(RbObject* x) {
 
    if ( x->isTypeSpec( TypeSpec(RbBoolean_name) ) ) {
        elements.push_back(static_cast<RbBoolean*>( x )->getValue());
    } else if ( x->isConvertibleTo(RbBoolean_name) ) {
        elements.push_back(static_cast<RbBoolean*>(x->convertTo(RbBoolean_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbBoolean_name + "[] with invalid value" );
    }
}

/** Append element to end of vector, updating length in process */
void VectorBoolean::push_back(bool x) {

    elements.push_back( x );
}


/** Append element to front of vector, updating length in process */
void VectorBoolean::push_front(RbObject* x) {
    
    if ( x->isTypeSpec( TypeSpec(RbBoolean_name) ) ) {
        elements.insert( elements.begin() , static_cast<RbBoolean*>( x )->getValue());
    } else if ( x->isConvertibleTo(RbBoolean_name) ) {
        elements.insert( elements.begin() , static_cast<RbBoolean*>(x->convertTo(RbBoolean_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbBoolean_name + "[] with invalid value" );
    }
}

/** Add element in front of vector, updating length in process */
void VectorBoolean::push_front(bool x) {
    
    elements.insert(elements.begin(), x );
}


void VectorBoolean::resize(size_t n) {
    elements.resize(n);
}


void VectorBoolean::setElement(const size_t index, RbLanguageObject* x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(RbBoolean_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<RbBoolean*>( x )->getValue());
    } else if ( x->isConvertibleTo(RbBoolean_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<RbBoolean*>(x->convertTo(RbBoolean_name))->getValue());
        // since we own the parameter, we delete the old type
        delete x;
    }
    else {
        throw RbException( "Trying to set " + RbBoolean_name + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorBoolean::setValue(const std::vector<bool>& x) {

    elements = x;
}   


/** Set value of vector using VectorBoolean */
void VectorBoolean::setValue(const VectorBoolean& x) {

    elements = x.getValue();
}   


size_t VectorBoolean::size(void) const {
    return elements.size();
}


/** Sort the vector */
void VectorBoolean::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/** Remove consecutive duplicates and resizes the vector */
void VectorBoolean::unique(void) {
    std::vector<bool> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    elements = uniqueVector;
    return;
    
}
