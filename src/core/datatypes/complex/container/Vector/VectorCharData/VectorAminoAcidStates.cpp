/**
 * @file
 * This file contains the implementation of VectorAminoAcidStates, a complex type
 * used to hold a string of Standard characters.
 *
 * @brief Implementation of VectorAminoAcidStates
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends VectorCharacters
 *
 * $Id$
 */

#include "AminoAcidState.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorAminoAcidStates.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorAminoAcidStates::VectorAminoAcidStates(void) : VectorCharacters(AminoAcidState_name) {
    
}


/** Copy constructor */
VectorAminoAcidStates::VectorAminoAcidStates(const VectorAminoAcidStates& x) : VectorCharacters(AminoAcidState_name) {

    for (size_t i=0; i<x.getLength(); i++)
        elements.push_back( new AminoAcidState(x[i]) );
    length = elements.size();
}


/** Subscript operator */
AminoAcidState& VectorAminoAcidStates::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<AminoAcidState*>(elements[i]) );
}


/** Subscript const operator */
const AminoAcidState& VectorAminoAcidStates::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<AminoAcidState*>(elements[i]) );
}


/** Equals comparison */
bool VectorAminoAcidStates::operator==(const VectorAminoAcidStates& x) const {

    if ( getLength() != x.getLength() )
        return false;
    for (size_t i=0; i<elements.size(); i++) 
        {
        if ( operator[](i) != x[i] )
            return false;
        }
    return true;
}


/** Not equals comparison */
bool VectorAminoAcidStates::operator!=(const VectorAminoAcidStates& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorAminoAcidStates) */
VectorAminoAcidStates VectorAminoAcidStates::operator+(const VectorAminoAcidStates& x) const {

    VectorAminoAcidStates tempVec = *this;
    for (size_t i=0; i<x.getLength(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (AminoAcidState) */
VectorAminoAcidStates VectorAminoAcidStates::operator+(const AminoAcidState& x) const {

    VectorAminoAcidStates tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorAminoAcidStates* VectorAminoAcidStates::clone(void) const {

    return new VectorAminoAcidStates(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorAminoAcidStates::getClass(void) const {

    static VectorString rbClass = VectorString(VectorAminoAcidStates_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<AminoAcidState*> VectorAminoAcidStates::getStdVector(void) const {	 

    std::vector<AminoAcidState*> dnaVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        AminoAcidState* d = static_cast<AminoAcidState*>( elements.at(i) );
        dnaVector.push_back(d);	 
        }	 
    return dnaVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorAminoAcidStates::push_back(AminoAcidState x) {

    elements.push_back( new AminoAcidState(x) );
    length++;
}


/** Print info about object */
void VectorAminoAcidStates::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorAminoAcidStates::richInfo(void) const {

    std::ostringstream o;
    o << "VectorAminoAcidStates: ";
    printValue(o);
    return o.str();
}

