/**
 * @file
 * This file contains the implementation of VectorCharacterObservations, an
 * abstract base class for different types of character observation vectors.
 *
 * @brief Implementation of VectorCharacterObservations
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

#include "Character.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorCharacters.h"



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

