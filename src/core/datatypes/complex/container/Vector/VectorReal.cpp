/**
 * @file
 * This file contains the implementation of VectorReal, a complex type
 * used to hold double vectors.
 *
 * @brief Implementation of VectorReal
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
#include "Real.h"
#include "VectorReal.h"
#include "VectorRealPos.h"
#include "VectorString.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>


// Definition of the static type spec member
const TypeSpec VectorReal::typeSpec(VectorReal_name);

/** Default constructor */
VectorReal::VectorReal(void) : AbstractVector(Real_name) {
}


/** Construct vector with one double x */
VectorReal::VectorReal(const double x) : AbstractVector(Real_name) {
    
    elements.push_back( x );
}


/** Construct vector with n doubles x */
VectorReal::VectorReal(const size_t n, const double x) : AbstractVector(Real_name) {

    for (size_t i = 0; i < n; i++) {
        elements.push_back( x );
    }
}


/** Constructor from double vector */
VectorReal::VectorReal(const std::vector<double>& x) : AbstractVector(Real_name) {

    elements = x;
}


/** Constructor from VectorRealPos */
VectorReal::VectorReal( const VectorRealPos& x ) : AbstractVector( x ) {

    elements = x.getValue();
}


/** Subscript operator */
double& VectorReal::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return elements[i];
}


/** Subscript const operator */
const double& VectorReal::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return elements[i];
}


/** Equals comparison */
bool VectorReal::operator==(const VectorReal& x) const {

    if (size() != x.size())
        return false;

    for (size_t i=0; i<elements.size(); i++) {
        if (operator[](i) != x[i])
            return false;
    }

    return true;
}


/** Not equals comparison */
bool VectorReal::operator!=(const VectorReal& x) const {

    return !operator==(x);
}


void VectorReal::clear(void) {
    elements.clear();
}


/** Clone function */
VectorReal* VectorReal::clone(void) const {

    return new VectorReal(*this);
}


/** Can we convert this vector into another object? */
RbObject* VectorReal::convertTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        return new VectorRealPos( getValue() );
    }
    
    return AbstractVector::convertTo(type);
}


/** Get class vector describing type of object */
const VectorString& VectorReal::getClass(void) const {

    static VectorString rbClass = VectorString(VectorReal_name) + AbstractVector::getClass();
    return rbClass;
}


RbPtr<const RbObject> VectorReal::getElement(size_t index) const {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<const RbObject> n( new Real(elements[index]) );
    
    return n;
}


RbPtr<RbObject> VectorReal::getElement(size_t index) {
    
    if (index > elements.size())
        throw RbException("Index out of bounds");
    
    RbPtr<RbObject> n( new Real(elements[index]) );
    
    return n;
}

/** Export value as STL vector */
std::vector<double> VectorReal::getValue(void) const {

    return elements;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& VectorReal::getTypeSpec(void) const {
    return typeSpec;
}


/** Can we convert this vector into another object? */
bool VectorReal::isConvertibleTo(TypeSpec const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        for (std::vector<double>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            
            // test whether we can convert this element, otherwise return false
            if (*it < 0) {
                return false;
            }
        }
        
        return true;
    }
    
    return AbstractVector::isConvertibleTo(type);
}


void VectorReal::pop_back(void) {
    elements.pop_back();
}


void VectorReal::pop_front(void) {
    
    elements.erase(elements.begin());
}


/** Print value for user */
void VectorReal::printValue(std::ostream& o) const {

    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o << "[ ";
    o << std::fixed;
    o << std::setprecision(1);
    for (std::vector<double>::const_iterator i = elements.begin(); i!= elements.end(); i++) 
        {
        if (i != elements.begin())
            o << ", ";
        o << (*i);
        }
    o <<  " ]";

    o.setf(previousFlags);
    o.precision(previousPrecision);
}


/** Push an int onto the back of the vector after checking */
void VectorReal::push_back( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Real_name) ) ) {
        elements.push_back(static_cast<Real*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Real_name) ) {
        elements.push_back(static_cast<Real*>(x->convertTo(Real_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Real_name + "[] with invalid value" );
    }
}

/** Append element to end of vector, updating length in process */
void VectorReal::push_back(double x) {
    
    elements.push_back( x );
}


/** Push an int onto the front of the vector after checking */
void VectorReal::push_front( RbPtr<RbObject> x ) {
    
    if ( x->isTypeSpec( TypeSpec(Real_name) ) ) {
        elements.insert( elements.begin(), static_cast<Real*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Real_name) ) {
        elements.insert( elements.begin(), static_cast<Real*>(x->convertTo(Real_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Real_name + "[] with invalid value" );
    }
}


/** Add element in front of vector, updating length in process */
void VectorReal::push_front(double x) {
    
    elements.insert(elements.begin(), x );
}


void VectorReal::resize(size_t n) {
    elements.resize(n);
}


/** Complete info about object */
std::string VectorReal::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorReal: value = ";
    printValue(o);

    return o.str();
}


void VectorReal::setElement(const size_t index, RbPtr<RbLanguageObject> x) {
    
    // check for type and convert if necessary
    if ( x->isTypeSpec( TypeSpec(Real_name) ) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        elements.insert( elements.begin() + index, static_cast<Real*>(x.get())->getValue());
    } else if ( x->isConvertibleTo(Real_name) ) {
        // resize if necessary
        if (index >= elements.size()) {
            elements.resize(index);
        }
        
        // remove first the old element at the index
        elements.erase(elements.begin()+index);
        
        elements.insert( elements.begin() + index, static_cast<Real*>(x->convertTo(Real_name))->getValue());
    }
    else {
        throw RbException( "Trying to set " + Real_name + "[] with invalid value" );
    }
}


/** Set value of vector using STL vector */
void VectorReal::setValue(const std::vector<double>& x) {

    elements = x;
}   


/** Set value of vector using VectorReal */
void VectorReal::setValue(const VectorReal& x) {

    elements = x.elements;
}     


size_t VectorReal::size(void) const {
    return elements.size();
}


/* Sort the vector */
void VectorReal::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove consecutive duplicates and resizes the vector */
void VectorReal::unique(void) {
    sort();
    std::vector<double> uniqueVector;
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

