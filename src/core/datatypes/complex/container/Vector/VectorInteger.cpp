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

#include "ContainerIterator.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VectorReal.h"
#include "VectorRealPos.h"

#include <sstream>



/** Default constructor */
VectorInteger::VectorInteger(void) : Vector(Integer_name) {
}


/** Construct vector with one int x */
VectorInteger::VectorInteger(int x) : Vector(Integer_name) {
    
    Integer *element = new Integer(x);
    element->retain();
    elements.push_back( element );
    
    length++;
}


/** Construct vector with n ints x */
VectorInteger::VectorInteger(size_t n, int x) : Vector(Integer_name) {

    for (size_t i = 0; i < n; i++) {
        Integer *element = new Integer(x);
        element->retain();
        elements.push_back( element );
    }
    
    length = n;
}


/** Constructor from int vector */
VectorInteger::VectorInteger(const std::vector<int>& x) : Vector(Integer_name) {

    for (size_t i=0; i<x.size(); i++) {
        Integer *element = new Integer(x[i]);
        element->retain();
        elements.push_back( element );
    }
    
    length = x.size();
}


/** Constructor from VectorInteger */
VectorInteger::VectorInteger(const VectorNatural& x) : Vector(Integer_name) {

    for (size_t i=0; i<x.getLength(); i++) {
        Integer *element = new Integer(x[i]);
        element->retain();
        elements.push_back( element );
    }
    
    length = x.getLength();
}


/** Constructor from size_t vector */
VectorInteger::VectorInteger(const std::vector<size_t>& x) : Vector(Integer_name) {

    for (std::vector<size_t>::const_iterator i=x.begin(); i!=x.end(); i++) {
        Integer *element = new Integer(int(*i));
        element->retain();
        elements.push_back( element );
    }
    
    length = elements.size();
}


/** Constructor from container iterator */
VectorInteger::VectorInteger(const ContainerIterator& x) : Vector(Integer_name) {

    for (ContainerIterator::const_iterator i=x.begin(); i!=x.end(); i++) {
        Integer *element = new Integer(*i);
        element->retain();
        elements.push_back( element );
    }
    
    length = elements.size();
}


/** Subscript operator */
int& VectorInteger::operator[](size_t i) {

    if (i > elements.size())
        throw RbException("Index out of bounds");

    return static_cast<Integer*>(elements[i])->getValueReference();
}


/** Subscript const operator */
const int& VectorInteger::operator[](size_t i) const {

    if (i >= int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<Integer*>(elements[i])->getValueReference();
}


/** Equals comparison */
bool VectorInteger::operator==(const VectorInteger& x) const {

    if (getLength() != x.getLength())
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


/** Clone function */
VectorInteger* VectorInteger::clone() const {

    return new VectorInteger(*this);
}


/** Can we convert this vector into another object? */
RbLanguageObject* VectorInteger::convertTo(std::string const &type) const {
    
    // test for type conversion
    if (type == VectorRealPos_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            int i = dynamic_cast<Natural*>(*it)->getValue();
            d.push_back(i);
        }
        
        return new VectorRealPos(d);
    }
    else if (type == VectorNatural_name) {
        
        return new VectorNatural( getValue() );
    }
    else if (type == VectorReal_name) {
        
        // create an stl vector and add each element
        std::vector<double> d;
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            int i = dynamic_cast<Natural*>(*it)->getValue();
            d.push_back(i);
        }
        
        return new VectorRealPos(d);
    }
    
    return Vector::convertTo(type);
}

/** Get class vector describing type of object */
const VectorString& VectorInteger::getClass() const {

    static VectorString rbClass = VectorString(VectorInteger_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<int> VectorInteger::getValue(void) const {

    std::vector<int> temp;
    for (size_t i=0; i<getLength(); i++)
        temp.push_back(operator[](i));

    return temp;
}


/** Can we convert this vector into another object? */
bool VectorInteger::isConvertibleTo(std::string const &type, bool once) const {
    
    // test for type conversion
    if (type == VectorNatural_name && once == true) {
        
        for (std::vector<RbLanguageObject*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            Integer *x = dynamic_cast<Integer*>(*it);
            
            // test whether we can convert this element, otherwise return false
            if (!x->isConvertibleTo(type, once)) {
                return false;
            }
        }
        
        return true;
    }
    else if (type == VectorReal_name || type == VectorRealPos_name) {
        
        return true;
    }
    
    return Vector::isConvertibleTo(type, once);
}


/** Append element to end of vector, updating length in process */
void VectorInteger::push_back(int x) {
    
    Integer *element = new Integer(x);
    element->retain();
    elements.push_back( element );
    
    length++;
}


/** Add element in front of vector, updating length in process */
void VectorInteger::push_front(int x) {
    
    Integer *element = new Integer(x);
    element->retain();
    elements.insert(elements.begin(), element);
    
    length++;
}


/** Complete info about object */
std::string VectorInteger::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorInteger: value = ";
    printValue(o);

    return o.str();
}


/** Set value of vector using STL vector */
void VectorInteger::setValue(const std::vector<int>& x) {

    clear();
    for (std::vector<int>::const_iterator i=x.begin(); i!=x.end(); i++) { 
        Integer *element = new Integer(*i);
        element->retain();
        elements.push_back( element );
    }
    
    length = x.size();
}   


/** Set value of vector using VectorInteger */
void VectorInteger::setValue(const VectorInteger& x) {

    clear();
    for (size_t i=0; i<x.getLength(); i++) { 
        Integer *element = new Integer(x[i]);
        element->retain();
        elements.push_back( element );
    }
    
    length = x.getLength();
}   

