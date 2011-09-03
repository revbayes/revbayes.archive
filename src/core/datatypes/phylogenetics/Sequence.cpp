/**
 * @file
 * This file contains the implementation of Sequence, an
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
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "Sequence.h"


// Definition of the static type spec member
const TypeSpec Sequence::typeSpec(Sequence_name);

/** Constructor with element type, used to properly construct vectors */
Sequence::Sequence(const std::string& elemType, const std::string tname) : ConstantMemberObject(), characters(elemType), taxonName(tname), characterType(elemType) {
    
}


/** Subscript operator */
Character& Sequence::operator[](size_t i) {
    
    if (i >= characters.size())
        throw RbException("Index out of bounds");
    return static_cast<Character&>(characters[i]);
}


/** Subscript const operator */
const Character& Sequence::operator[](size_t i) const {
    
    if (i >= characters.size())
        throw RbException("Index out of bounds");
    return static_cast<const Character&>(characters[i]);
}

/** Push back a new character */
void Sequence::addCharacter( Character* newChar ) {
    
    if ( newChar == NULL || !newChar->isTypeSpec( characterType ) )
        throw RbException( "Inappropriate character type" );
    
    characters.push_back( newChar );
}


Sequence* Sequence::clone(void) const {
    return new Sequence(*this);
}


/** Get class vector describing type of object */
const VectorString& Sequence::getClass(void) const {
    
    static VectorString rbClass = VectorString(Sequence_name) + ConstantMemberObject::getClass();
    return rbClass;
}


Character* Sequence::getCharacter(size_t index) {
    return &static_cast<Character&>(characters[index]);
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Sequence::getTypeSpec(void) const {
    return typeSpec;
}


void Sequence::printValue(std::ostream &o) const {
    o << taxonName << ":";
    characters.printValue(o);
}
