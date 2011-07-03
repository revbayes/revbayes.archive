/**
 * @file
 * This file contains the implementation of VectorRnaStates, a complex type
 * used to hold a string of RNA.
 *
 * @brief Implementation of VectorRnaStates
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

#include "RnaState.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorRnaStates.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorRnaStates::VectorRnaStates(void) : VectorCharacters(DnaState_name) {
    
}


/** Copy constructor */
VectorRnaStates::VectorRnaStates(const VectorRnaStates& x) : VectorCharacters(DnaState_name) {

    for (size_t i=0; i<x.size(); i++)
        elements.push_back( new RnaState(x[i]) );
    length[0] = elements.size();
}


/** Subscript operator */
RnaState& VectorRnaStates::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<RnaState*>(elements[i]) );
}


/** Subscript const operator */
const RnaState& VectorRnaStates::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<RnaState*>(elements[i]) );
}


/** Equals comparison */
bool VectorRnaStates::operator==(const VectorRnaStates& x) const {

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
bool VectorRnaStates::operator!=(const VectorRnaStates& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorRnaStates) */
VectorRnaStates VectorRnaStates::operator+(const VectorRnaStates& x) const {

    VectorRnaStates tempVec = *this;
    for (size_t i=0; i<x.size(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (DnaState) */
VectorRnaStates VectorRnaStates::operator+(const RnaState& x) const {

    VectorRnaStates tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorRnaStates* VectorRnaStates::clone(void) const {

    return new VectorRnaStates(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorRnaStates::getClass(void) const {

    static VectorString rbClass = VectorString(VectorRnaStates_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<RnaState*> VectorRnaStates::getStdVector(void) const {	 

    std::vector<RnaState*> rnaVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        RnaState* d = static_cast<RnaState*>( elements.at(i) );
        rnaVector.push_back(d);	 
        }	 
    return rnaVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorRnaStates::push_back(RnaState x) {

    elements.push_back( new RnaState(x) );
    length[0]++;
}


/** Print info about object */
void VectorRnaStates::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorRnaStates::richInfo(void) const {

    std::ostringstream o;
    o << "VectorRnaStates: ";
    printValue(o);
    return o.str();
}

