/**
 * @file
 * This file contains the implementation of VectorStandardStates, a complex type
 * used to hold a string of Standard characters.
 *
 * @brief Implementation of VectorStandardStates
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

#include "StandardState.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorStandardStates.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorStandardStates::VectorStandardStates(void) : VectorCharacters(StandardState_name) {
    
}


/** Copy constructor */
VectorStandardStates::VectorStandardStates(const VectorStandardStates& x) : VectorCharacters(StandardState_name) {

    for (size_t i=0; i<x.getLength(); i++) {
        StandardState *element = new StandardState(x[i]);
        element->retain();
        elements.push_back( element );
    }
    
    length = elements.size();
}


/** Subscript operator */
StandardState& VectorStandardStates::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<StandardState*>(elements[i]) );
}


/** Subscript const operator */
const StandardState& VectorStandardStates::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<StandardState*>(elements[i]) );
}


/** Equals comparison */
bool VectorStandardStates::operator==(const VectorStandardStates& x) const {

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
bool VectorStandardStates::operator!=(const VectorStandardStates& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorStandardStates) */
VectorStandardStates VectorStandardStates::operator+(const VectorStandardStates& x) const {

    VectorStandardStates tempVec = *this;
    for (size_t i=0; i<x.getLength(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (StandardState) */
VectorStandardStates VectorStandardStates::operator+(const StandardState& x) const {

    VectorStandardStates tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorStandardStates* VectorStandardStates::clone(void) const {

    return new VectorStandardStates(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorStandardStates::getClass(void) const {

    static VectorString rbClass = VectorString(VectorStandardStates_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<StandardState*> VectorStandardStates::getStdVector(void) const {	 

    std::vector<StandardState*> dnaVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        StandardState* d = static_cast<StandardState*>( elements.at(i) );
        dnaVector.push_back(d);	 
        }	 
    return dnaVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorStandardStates::push_back(StandardState x) {
    
    StandardState *element = new StandardState(x);
    element->retain();
    elements.push_back( element );
    
    length++;
}


/** Print info about object */
void VectorStandardStates::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorStandardStates::richInfo(void) const {

    std::ostringstream o;
    o << "VectorStandardStates: ";
    printValue(o);
    return o.str();
}

