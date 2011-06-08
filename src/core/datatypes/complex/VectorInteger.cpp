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
 * @extends RbComplex
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

#include <sstream>



/** Default constructor */
VectorInteger::VectorInteger(void)
    : Vector(Integer_name) {
}


/** Construct vector with one int x */
VectorInteger::VectorInteger(int x)
    : Vector(Integer_name) {

    elements.push_back(new Integer(x));
    length[0]++;
}


/** Construct vector with n ints x */
VectorInteger::VectorInteger(size_t n, int x)
    : Vector(Integer_name) {

    for (size_t i = 0; i < n; i++)
        elements.push_back(new Integer(x));
    length[0] = n;
}


/** Constructor from int vector */
VectorInteger::VectorInteger(const std::vector<int>& x)
    : Vector(Integer_name) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back(new Integer(x[i]));
        length[0]++;
    }
}


/** Constructor from VectorInteger */
VectorInteger::VectorInteger(const VectorNatural& x)
    : Vector(Integer_name) {

    for (size_t i=0; i<x.size(); i++) {
        elements.push_back(new Integer(x[i]));
        length[0]++;
    }
}


/** Constructor from size_t vector */
VectorInteger::VectorInteger(const std::vector<size_t>& x)
    : Vector(Integer_name) {

    for (std::vector<unsigned int>::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Integer(int(*i)));
    length[0] = elements.size();
}


/** Constructor from container iterator */
VectorInteger::VectorInteger(const ContainerIterator& x)
: Vector(Integer_name) {

    for (ContainerIterator::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new Integer(*i));
    length[0] = elements.size();
}


/** Subscript operator */
int& VectorInteger::operator[](size_t i) {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");

    return static_cast<Integer*>(elements[i])->getValueRef();
}


/** Subscript const operator */
const int& VectorInteger::operator[](size_t i) const {

    if (i < 0 || i > int(elements.size()))
        throw RbException("Index out of bounds");
    return static_cast<Integer*>(elements[i])->getValueRef();
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


/** Clone function */
VectorInteger* VectorInteger::clone() const {

    return new VectorInteger(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorInteger::getClass() const {

    static VectorString rbClass = VectorString(VectorInteger_name) + Vector::getClass();
    return rbClass;
}


/** Export value as STL vector */
std::vector<int> VectorInteger::getValue(void) const {

    std::vector<int> temp;
    for (size_t i=0; i<size(); i++)
        temp.push_back(operator[](i));

    return temp;
}


/** Append element to end of vector, updating length in process */
void VectorInteger::push_back(int x) {

    elements.push_back(new Integer(x));
    length[0]++;
}


/** Add element in front of vector, updating length in process */
void VectorInteger::push_front(int x) {

    elements.insert(elements.begin(), new Integer(x));
    length[0]++;
}


/** Complete info about object */
std::string VectorInteger::richInfo(void) const {

    std::ostringstream o;
    o <<  "VectorInteger: value = ";
    printValue(o);

    return o.str();
}


/** Convert to element index string for use in parser */
std::string VectorInteger::toIndexString(void) const {

    std::ostringstream o;
    for ( size_t i = 0; i < elements.size(); i++ ) {
        if ( operator[](i) < 0 )
            o <<  "[]";
        else
            o << "[" << operator[](i) + 1 << "]";
    }

    return o.str();
}


/** Set value of vector using STL vector */
void VectorInteger::setValue(const std::vector<int>& x) {

    clear();
    for (std::vector<int>::const_iterator i=x.begin(); i!=x.end(); i++) {   
        elements.push_back(new Integer(*i));
        length[0]++;
    }
}   


/** Set value of vector using VectorInteger */
void VectorInteger::setValue(const VectorInteger& x) {

    clear();
    for (size_t i=0; i<x.size(); i++) {   
        elements[i] = new Integer(x[i]);
        length[0]++;
    }
}   

