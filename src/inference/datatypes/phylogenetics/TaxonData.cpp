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


#include "CharacterState.h"
#include "ConstArgumentRule.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "SimpleMemberFunction.h"
#include "TaxonData.h"


/** Constructor with element type, used to properly construct vectors */
TaxonData::TaxonData(const std::string &t, const std::string tname) : characterType( t ), taxonName(tname), sequence() {
    
}


/** Subscript operator */
CharacterState& TaxonData::operator[](size_t i) {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    return *sequence[i];
}


/** Subscript const operator */
const CharacterState& TaxonData::operator[](size_t i) const {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    return *sequence[i];
}

/** Push back a new character */
void TaxonData::addCharacter( CharacterState *newChar ) {
    
    sequence.push_back( newChar );
}


CharacterState& TaxonData::getCharacter(size_t index) {
    
    return operator[](index);
}


const CharacterState& TaxonData::getCharacter(size_t index) const {

    return operator[](index);
}


size_t TaxonData::getNumberOfCharacters(void) const {

    return sequence.size();
}


const std::string& TaxonData::getTaxonName(void) const {

    return taxonName;
}



void TaxonData::setTaxonName(std::string tn) {

    taxonName = tn;
}

