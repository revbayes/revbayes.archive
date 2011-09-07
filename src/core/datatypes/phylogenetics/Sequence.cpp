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
Sequence::Sequence(const std::string& elemType, const std::string tname) : AbstractVector(elemType), characters(elemType), taxonName(tname) {
    
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
    
    if ( newChar == NULL || !newChar->isTypeSpec( characters.getElementType() ) )
        throw RbException( "Inappropriate character type" );
    
    characters.push_back( newChar );
}


void Sequence::clear(void) {
    characters.clear();
}


Sequence* Sequence::clone(void) const {
    return new Sequence(*this);
}


/** Get class vector describing type of object */
const VectorString& Sequence::getClass(void) const {
    
    static VectorString rbClass = VectorString(Sequence_name) + ConstantMemberObject::getClass();
    return rbClass;
}


Character* Sequence::getCharacter(size_t index) const {
    return static_cast<Character*>(characters.getElement(index));
}


Character* Sequence::getElement(size_t index) const {
    return static_cast<Character*>(characters.getElement(index));
}


const std::string& Sequence::getTaxonName(void) const {
    return taxonName;
}



/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Sequence::getTypeSpec(void) const {
    return typeSpec;
}


void Sequence::pop_back(void) {
    characters.pop_back();
}


void Sequence::pop_front(void) {
    characters.pop_front();
}


void Sequence::printValue(std::ostream &o) const {
    o << taxonName << ":" << std::endl;
    characters.printValue(o);
}


void Sequence::push_back(RbObject *x) {
    characters.push_back(x);
}


void Sequence::push_front(RbObject *x) {
    characters.push_front(x);
}


void Sequence::resize(size_t n) {
    characters.resize(n);
}


/** Print value for user */
std::string Sequence::richInfo(void) const {
    
    // get the vector of characters
    std::vector<RbLanguageObject*> elements = characters.getValue();
    
    std::string info = taxonName + " [ ";
    for ( std::vector<RbLanguageObject*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            info += ", ";
        if ( (*i) == NULL )
            info += "NULL";
        else 
            info += (*i)->richInfo();
    }
    info +=  " ]";
    
    return info;
}


void Sequence::setElement(const size_t index, RbLanguageObject *elem) {
    characters.setElement(index, elem);
}


void Sequence::setTaxonName(std::string tn) {
    taxonName = tn;
}


size_t Sequence::size(void) const {
    return characters.size();
}


void Sequence::sort(void) {
    characters.sort();
}


void Sequence::unique(void) {
    characters.unique();
}
