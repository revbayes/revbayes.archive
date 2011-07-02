/**
 * @file
 * This file contains the implementation of VectorContinuousObservations, a complex type
 * used to hold a string of Standard characters.
 *
 * @brief Implementation of VectorContinuousObservations
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

#include "CharacterContinuous.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorContinuousObservations.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorContinuousObservations::VectorContinuousObservations(void) : VectorCharacterObservations(CharacterContinuous_name) {
    
}


/** Copy constructor */
VectorContinuousObservations::VectorContinuousObservations(const VectorContinuousObservations& x) : VectorCharacterObservations(CharacterContinuous_name) {

    for (size_t i=0; i<x.size(); i++)
        elements.push_back( new CharacterContinuous(x[i]) );
    length[0] = elements.size();
}


/** Subscript operator */
CharacterContinuous& VectorContinuousObservations::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<CharacterContinuous*>(elements[i]) );
}


/** Subscript const operator */
const CharacterContinuous& VectorContinuousObservations::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<CharacterContinuous*>(elements[i]) );
}


/** Equals comparison */
bool VectorContinuousObservations::operator==(const VectorContinuousObservations& x) const {

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
bool VectorContinuousObservations::operator!=(const VectorContinuousObservations& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorContinuousObservations) */
VectorContinuousObservations VectorContinuousObservations::operator+(const VectorContinuousObservations& x) const {

    VectorContinuousObservations tempVec = *this;
    for (size_t i=0; i<x.size(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (StandardState) */
VectorContinuousObservations VectorContinuousObservations::operator+(const CharacterContinuous& x) const {

    VectorContinuousObservations tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorContinuousObservations* VectorContinuousObservations::clone(void) const {

    return new VectorContinuousObservations(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorContinuousObservations::getClass(void) const {

    static VectorString rbClass = VectorString(VectorContinuousObservations_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<CharacterContinuous*> VectorContinuousObservations::getStdVector(void) const {	 

    std::vector<CharacterContinuous*> continuousVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        CharacterContinuous* d = static_cast<CharacterContinuous*>( elements.at(i) );
        continuousVector.push_back(d);	 
        }	 
    return continuousVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorContinuousObservations::push_back(CharacterContinuous x) {

    elements.push_back( new CharacterContinuous(x) );
    length[0]++;
}


/** Print info about object */
void VectorContinuousObservations::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorContinuousObservations::richInfo(void) const {

    std::ostringstream o;
    o << "VectorContinuousObservations: ";
    printValue(o);
    return o.str();
}

