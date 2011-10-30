/**
 * @file
 * This file contains the implementation of VectorString, a complex type
 * used to hold string vectors.
 *
 * @brief Implementation of VectorString
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

#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <algorithm>
#include <sstream>


// Definition of the static type spec member
const TypeSpec VectorString::typeSpec(VectorString_name);

/** Construct empty string vector */
VectorString::VectorString(void) : AbstractVector(RbString_name) {
}


/** Construct vector with one string x */
VectorString::VectorString(const std::string& x) : AbstractVector(RbString_name) {

    elements.push_back( x );
}


/** Constructor from std::string vector */
VectorString::VectorString(const std::vector<std::string>& x) : AbstractVector(RbString_name) {

    elements = x;
}


/** Subscript operator */
std::string& VectorString::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const std::string& VectorString::operator[](size_t i) const {

    if (i > elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorString::operator==(const VectorString& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorString::operator!=(const VectorString& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (RbString) */
VectorString VectorString::operator+( const RbString& x ) const {

    VectorString tempVec = *this;

    tempVec.push_back( x.getValue() );

    return tempVec;
}


/** Concatenation with operator+ (VectorString) */
VectorString VectorString::operator+( const VectorString& x ) const {

    VectorString tempVec = *this;

    for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );

    return tempVec;
}


/** Concatenation with operator+ (std::string) */
VectorString VectorString::operator+( const std::string& x ) const {

    VectorString tempVec = *this;

    tempVec.push_back( x );

    return tempVec;
}


void VectorString::clear(void) {
    elements.clear();
}


/** Clone function */
VectorString* VectorString::clone() const {

    return new VectorString(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorString::getClass() const {

    static VectorString rbClass = VectorString(VectorString_name) + AbstractVector::getClass();
    return rbClass;
}


RbPtr<const RbObject> VectorString::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<const RbObject> n(new RbString(elements[index]));
    
    return n;
}


RbPtr<RbObject> VectorString::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<RbObject> n(new RbString(elements[index]));
    
    return n;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorString::getTypeSpec(void) const {
    return typeSpec;
}


/** Get STL vector of strings */
std::vector<std::string> VectorString::getValue(void) const {	 

    return elements;	 
}


void VectorString::pop_back(void) {
    elements.pop_back();
}


void VectorString::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Push an int onto the back of the vector after checking */
void VectorString::push_back( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(RbString_name) ) ) {
        elements.push_back(static_cast<RbString*>(x.get())->getValue() );
    } else if ( x->isConvertibleTo(RbString_name) ) {
        elements.push_back(static_cast<RbString*>(x->convertTo(RbString_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + RbString_name + "[] with invalid value" );
    }
}


/** Append string element to end of vector, updating length in process */
void VectorString::push_back(const std::string &x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorString::push_front( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(RbString_name) ) ) {
        elements.insert( elements.begin(), static_cast<RbString*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(RbString_name) ) {
        elements.insert( elements.begin(), static_cast<RbString*>(x->convertTo(RbString_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + RbString_name + "[] with invalid value" );
    }
}


void VectorString::resize(size_t n) {
    elements.resize(n);
}


/** Complete info about object */
std::string VectorString::richInfo(void) const {

    std::ostringstream o;
    o << "VectorString: value = ";
    printValue(o);
    
    return o.str();
}


void VectorString::setElement(const size_t index, RbPtr<RbLanguageObject> x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(RbString_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<RbString*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(RbString_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<RbString*>(x->convertTo(RbString_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + RbString_name + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorString::setValue(const std::vector<std::string>& x) {
    
    elements = x;
}   


/** Set value of vector using VectorString */
void VectorString::setValue(const VectorString& x) {
    
    elements = x.elements;
}   


size_t VectorString::size(void) const {
    return elements.size();
}


/** Sort the vector */
void VectorString::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/** Remove consecutive duplicates and resizes the vector */
void VectorString::unique(void) {
    
    sort();
    std::vector<std::string> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    elements = uniqueVector;
    return;
    
}

