/**
 * @file
 * This file contains the implementation of VectorDnaStates, a complex type
 * used to hold a string of DNA.
 *
 * @brief Implementation of VectorDnaStates
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

#include "DnaState.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorDnaStates.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorDnaStates::VectorDnaStates(void) : Vector(VectorDnaStates_name) {
    
}


/** Copy constructor */
VectorDnaStates::VectorDnaStates(const VectorDnaStates& x) : Vector(VectorDnaStates_name) {

    for (size_t i=0; i<x.size(); i++)
        elements.push_back( new DnaState(x[i]) );
    length[0] = elements.size();
}


/** Subscript operator */
DnaState& VectorDnaStates::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");

    //return static_cast<DnaState*>(elements[i])->getValue();
}


/** Subscript const operator */
const DnaState& VectorDnaStates::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    //return static_cast<RbString*>(elements[i])->getValueRef();
}


/** Equals comparison */
bool VectorDnaStates::operator==(const VectorDnaStates& x) const {

    if ( size() != x.size() )
        return false;
    for (size_t i=0; i<elements.size(); i++) 
        {
        if (operator[](i) != x[i])
            return false;
        }
    return true;
}


/** Not equals comparison */
bool VectorDnaStates::operator!=(const VectorDnaStates& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (RbString) */
VectorDnaStates VectorDnaStates::operator+(const VectorDnaStates& x) const {

    //VectorString tempVec = *this;

    //tempVec.push_back( x.getValue() );

    //return tempVec;
}


/** Concatenation with operator+ (VectorString) */
VectorDnaStates VectorDnaStates::operator+(const DnaState& x) const {

    //VectorString tempVec = *this;

    /*for ( size_t i = 0; i < x.elements.size(); i++ )
        tempVec.push_back( x[i] );

    return tempVec;*/
}


/** Clone function */
VectorDnaStates* VectorDnaStates::clone() const {

    return new VectorDnaStates(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorDnaStates::getClass(void) const {

    static VectorString rbClass = VectorString(VectorDnaStates_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<std::string> VectorDnaStates::getStdVector(void) const {	 

    std::vector<std::string> strVector;	 

    for (size_t i=0; i<elements.size(); i++) {	 

        RbString* str = static_cast<RbString*>( elements.at(i) );
        strVector.push_back(str->getValue());	 
    }	 
    
    return strVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorDnaStates::push_back(DnaState x) {

    elements.push_back(new DnaState(x));
    length[0]++;
}


/** Complete info about object */
std::string VectorDnaStates::richInfo(void) const {

    std::ostringstream o;
    o << "VectorDnaStates: value = ";
    printValue(o);
    
    return o.str();
}

