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
    return *( static_cast<DnaState*>(elements[i]) );
}


/** Subscript const operator */
const DnaState& VectorDnaStates::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<DnaState*>(elements[i]) );
}


/** Equals comparison */
bool VectorDnaStates::operator==(const VectorDnaStates& x) const {

    if ( size() != x.size() )
        return false;
    for (size_t i=0; i<elements.size(); i++) 
        {
        if ( operator[](i) != x[i] )
            return false;
        }
    return true;
}


/** Not equals comparison */
bool VectorDnaStates::operator!=(const VectorDnaStates& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorDnaStates) */
VectorDnaStates VectorDnaStates::operator+(const VectorDnaStates& x) const {

    VectorDnaStates tempVec = *this;
    for (size_t i=0; i<x.size(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (DnaState) */
VectorDnaStates VectorDnaStates::operator+(const DnaState& x) const {

    VectorDnaStates tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorDnaStates* VectorDnaStates::clone(void) const {

    return new VectorDnaStates(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorDnaStates::getClass(void) const {

    static VectorString rbClass = VectorString(VectorDnaStates_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<DnaState*> VectorDnaStates::getStdVector(void) const {	 

    std::vector<DnaState*> dnaVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        DnaState* d = static_cast<DnaState*>( elements.at(i) );
        dnaVector.push_back(d);	 
        }	 
    return dnaVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorDnaStates::push_back(DnaState x) {

    elements.push_back( new DnaState(x) );
    length[0]++;
}


/** Print info about object */
void VectorDnaStates::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorDnaStates::richInfo(void) const {

    std::ostringstream o;
    o << "VectorDnaStates: value = ";
    printValue(o);
    return o.str();
}

