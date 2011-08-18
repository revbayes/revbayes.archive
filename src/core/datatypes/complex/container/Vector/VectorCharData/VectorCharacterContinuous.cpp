/**
 * @file
 * This file contains the implementation of VectorCharacterContinuous, a complex type
 * used to hold a string of Standard characters.
 *
 * @brief Implementation of VectorCharacterContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends VectorCharacter
 *
 * $Id$
 */

#include "CharacterContinuous.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorCharacterContinuous.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include <sstream>



/** Construct empty DNA vector */
VectorCharacterContinuous::VectorCharacterContinuous(void) : VectorCharacters(CharacterContinuous_name) {
    
}


/** Copy constructor */
VectorCharacterContinuous::VectorCharacterContinuous(const VectorCharacterContinuous& x) : VectorCharacters(CharacterContinuous_name) {

    for (size_t i=0; i<x.getLength(); i++) {
        CharacterContinuous *element = new CharacterContinuous(x[i]);
        element->retain();
        elements.push_back( element );
    }
    
    length = elements.size();
    
}


/** Subscript operator */
CharacterContinuous& VectorCharacterContinuous::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<CharacterContinuous*>(elements[i]) );
}


/** Subscript const operator */
const CharacterContinuous& VectorCharacterContinuous::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<CharacterContinuous*>(elements[i]) );
}


/** Equals comparison */
bool VectorCharacterContinuous::operator==(const VectorCharacterContinuous& x) const {

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
bool VectorCharacterContinuous::operator!=(const VectorCharacterContinuous& x) const {

    return !operator==(x);
}


/** Concatenation with operator+ (VectorCharacterContinuous) */
VectorCharacterContinuous VectorCharacterContinuous::operator+(const VectorCharacterContinuous& x) const {

    VectorCharacterContinuous tempVec = *this;
    for (size_t i=0; i<x.getLength(); i++)
        tempVec.push_back( x[i] );
    return tempVec;
}


/** Concatenation with operator+ (StandardState) */
VectorCharacterContinuous VectorCharacterContinuous::operator+(const CharacterContinuous& x) const {

    VectorCharacterContinuous tempVec = *this;
    tempVec.push_back( x );
    return tempVec;
}


/** Clone function */
VectorCharacterContinuous* VectorCharacterContinuous::clone(void) const {

    return new VectorCharacterContinuous(*this);
}


/** Get class vector describing type of object */
const VectorString& VectorCharacterContinuous::getClass(void) const {

    static VectorString rbClass = VectorString(VectorCharacterContinuous_name) + Vector::getClass();
    return rbClass;
}


/** Get STL vector of strings */
std::vector<CharacterContinuous*> VectorCharacterContinuous::getStdVector(void) const {	 

    std::vector<CharacterContinuous*> continuousVector;	 
    for (size_t i=0; i<elements.size(); i++) 
        {	 
        CharacterContinuous* d = static_cast<CharacterContinuous*>( elements.at(i) );
        continuousVector.push_back(d);	 
        }	 
    return continuousVector;	 
}


/** Append string element to end of vector, updating length in process */
void VectorCharacterContinuous::push_back(CharacterContinuous x) {
    
    CharacterContinuous *element = new CharacterContinuous(x);
    element->retain();
    elements.push_back( element );
    
    length++;
}


/** Print info about object */
void VectorCharacterContinuous::printValue(std::ostream& o) const {

    for (size_t i=0; i<elements.size(); i++)
        elements[i]->printValue(o);
}


/** Complete info about object */
std::string VectorCharacterContinuous::richInfo(void) const {

    std::ostringstream o;
    o << "VectorCharacterContinuous: ";
    printValue(o);
    return o.str();
}

