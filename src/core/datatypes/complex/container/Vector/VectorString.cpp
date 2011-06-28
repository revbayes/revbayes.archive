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
 * @extends RbComplex
 *
 * $Id:$
 */

#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <sstream>


/** Construct empty string vector */
VectorString::VectorString(void)
    : Vector(RbString_name) {
}


/** Construct vector with one string x */
VectorString::VectorString(const std::string& x)
    : Vector(RbString_name) {

    elements.push_back(new RbString(x));
    length[0] = 1;
}


/** Constructor from std::string vector */
VectorString::VectorString(const std::vector<std::string>& x) : Vector(RbString_name) {

    for (std::vector<std::string>::const_iterator i=x.begin(); i!=x.end(); i++)
        elements.push_back(new RbString(*i));
    length[0] = elements.size();
}


/** Subscript operator */
std::string& VectorString::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    return static_cast<RbString*>(elements[i])->getValueRef();
}


/** Subscript const operator */
const std::string& VectorString::operator[](size_t i) const {

    if (i > elements.size())
        throw RbException("Index out of bounds");
    return static_cast<RbString*>(elements[i])->getValueRef();
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


/** Clone function */
VectorString* VectorString::clone() const {

    return new VectorString(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorString::getClass() const {

    static VectorString rbClass = VectorString(VectorString_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<std::string> VectorString::getStdVector(void) const {	 

    std::vector<std::string> strVector;	 

    for (size_t i=0; i<elements.size(); i++) {	 

        RbString* str = static_cast<RbString*>( elements.at(i) );
        strVector.push_back(str->getValue());	 
    }	 
    
    return strVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorString::push_back(std::string x) {

    elements.push_back(new RbString(x));
    length[0]++;
}


/** Complete info about object */
std::string VectorString::richInfo(void) const {

    std::ostringstream o;
    o << "VectorString: value = ";
    printValue(o);
    
    return o.str();
}

