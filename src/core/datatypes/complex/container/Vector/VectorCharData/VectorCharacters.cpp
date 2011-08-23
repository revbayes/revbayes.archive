/**
 * @file
 * This file contains the implementation of VectorCharacters, an
 * abstract base class for different types of character vectors.
 *
 * @brief Implementation of VectorCharacterObservations
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

#include "Character.h"
#include "CharacterStateDiscrete.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "VectorCharacters.h"

#include <algorithm>


/** Constructor with element type, used to properly construct vectors */
VectorCharacters::VectorCharacters(const std::string& elemType) : Vector(elemType) {

}


/** Subscript operator */
Character& VectorCharacters::operator[](size_t i) {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<Character*>(elements[i]) );
}


/** Subscript const operator */
const Character& VectorCharacters::operator[](size_t i) const {

    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *( static_cast<Character*>(elements[i]) );
}


/** Get class vector describing type of object */
const VectorString& VectorCharacters::getClass(void) const {

    static VectorString rbClass = VectorString(VectorCharacters_name) + Vector::getClass();
    return rbClass;
}


Character* VectorCharacters::getElement(size_t index) {
    return static_cast<Character*>(elements[index]);
}

/** Push back a new character */
void VectorCharacters::push_back( Character* newChar ) {

    if ( newChar == NULL || !newChar->isType( elementType ) )
        throw RbException( "Inappropriate character type" );
    
    elements.push_back( newChar );
    newChar->retain();
    
    length++;
}

bool comparisonFunction (RbLanguageObject* i,RbLanguageObject* j) {

    return (*(static_cast<Character*>(i)) < *(static_cast<Character*>(j)) ); 

}


bool VectorCharacters::comparisonFunction (RbLanguageObject* i,RbLanguageObject* j) { 
    
    return (*(static_cast<Character*>(i)) < *(static_cast<Character*>(j)) ); 
    
}


/** Sort the vector */
void VectorCharacters::sort( void ) {
    
    std::sort(elements.begin(), elements.end(), comparisonFunction);
    return;
    
}

/* Remove duplicates and resize the vector */
void VectorCharacters::unique(void) {
    
    sort();
    std::vector<RbLanguageObject*> uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (*(static_cast<Character*>(elements[i])) != *(static_cast<Character*>(elements[i-1])))
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    length = elements.size();
    return;
    
}