/**
 * @file
 * This file contains the implementation of VectorBoolean, a complex type
 * used to hold vectors of Boolean values.
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

#include "Boolean.h"
#include "ContainerIterator.h"
#include "VectorBoolean.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>



/** Default constructor */
VectorBoolean::VectorBoolean(void) : Vector(Boolean_name) {

}


/** Construct vector with one bool x */
VectorBoolean::VectorBoolean(bool x) : Vector(Boolean_name) {

    Boolean *element = new Boolean(x);
    element->retain();
    elements.push_back( element );
    length = 1;
}


/** Construct vector with n bools x */
VectorBoolean::VectorBoolean(size_t n, bool x) : Vector(Boolean_name) {

    for (size_t i = 0; i < n; i++) {
        Boolean *element = new Boolean(x);
        element->retain();
        elements.push_back( element );
    }
    length = elements.size();
}


/** Constructor from bool vector */
VectorBoolean::VectorBoolean(const std::vector<bool>& x) : Vector(Boolean_name) {

    for (std::vector<bool>::const_iterator i=x.begin(); i!=x.begin(); i++) {
        Boolean *element = new Boolean(*i);
        element->retain();
        elements.push_back( element );
    }
    length = elements.size();
}


/** Constructor from int vector */
VectorBoolean::VectorBoolean(const std::vector<int>& x) : Vector(Boolean_name) {

    for (std::vector<int>::const_iterator i=x.begin(); i!=x.end(); i++) {
        Boolean *element = new Boolean( (*i) == 0 );
        element->retain();
        elements.push_back( element );
    }
    length = elements.size();
}

/** Subscript operator */
bool& VectorBoolean::operator[](size_t i) {

    if (i > elements.size())
        throw RbException("Index out of bounds");

    return static_cast<Boolean*>(elements[i])->getValueReference();
}


/** Subscript const operator */
const bool& VectorBoolean::operator[](size_t i) const {

    if ( i >= elements.size() )
        throw RbException("Index out of bounds");
    return static_cast<Boolean*>(elements[i])->getValueReference();
}


/** Equals comparison */
bool VectorBoolean::operator==(const VectorBoolean& x) const {

    if (getLength() != x.getLength())
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


/** Clone function */
VectorBoolean* VectorBoolean::clone() const {

    return new VectorBoolean(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorBoolean::getClass() const {

    static VectorString rbClass = VectorString(VectorBoolean_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<bool> VectorBoolean::getValue(void) const {

    std::vector<bool> temp;
    for (size_t i=0; i<getLength(); i++)
        temp.push_back(operator[](i));

    return temp;
}

/** Append element to end of vector, updating length in process */
void VectorBoolean::push_back(bool x) {
    
    Boolean *element = new Boolean(x);
    element->retain();
    elements.push_back( element );
    length++;
}


/** Add element in front of vector, updating length in process */
void VectorBoolean::push_front(bool x) {
    
    Boolean *element = new Boolean(x);
    element->retain();
    elements.insert(elements.begin(), element );
    length++;
}


/** Complete info about object */
std::string VectorBoolean::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorBoolean: value = ";
    printValue(o);

    return o.str();
}


/** Set value of vector using STL vector */
void VectorBoolean::setValue(const std::vector<bool>& x) {

    clear();
    for (std::vector<bool>::const_iterator i=x.begin(); i!=x.end(); i++) { 
        Boolean *element = new Boolean(*i);
        element->retain();
        elements.push_back( element );  
        length++;
    }
}   


/** Set value of vector using VectorBoolean */
void VectorBoolean::setValue(const VectorBoolean& x) {

    clear();
    for (size_t i=0; i<x.getLength(); i++) {   
        Boolean *element = new Boolean(x[i]);
        element->retain();
        elements.push_back( element );
        length++;
    }
}   

