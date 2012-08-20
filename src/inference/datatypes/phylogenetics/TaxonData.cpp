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
#include "ArgumentRule.h"
#include "CharacterState.h"
#include "Ellipsis.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbString.h"
#include "SimpleMemberFunction.h"
#include "TaxonData.h"

#include <algorithm>


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


bool TaxonData::operator==(const TaxonData &x) const {
    
    if ( characterType != x.characterType ) {
        return false;
    }
    
    if ( taxonName != x.taxonName ) {
        return false;
    }
    
    if ( sequence.size() != x.sequence.size() ) {
        return false;
    }
    
    for (size_t i = 0; i < sequence.size(); ++i) {
        if ( *sequence[i] != *x.sequence[i] ) {
            return false;
        }
    }
    
    return true;
}


bool TaxonData::operator!=(const TaxonData &x) const {
    return !operator==(x);
}


bool TaxonData::operator<(const TaxonData &x) const {
    
    for (size_t i = 0; i < sequence.size() && i < x.sequence.size(); ++i) {
        if ( *sequence[i] != *x.sequence[i] ) {
            return *sequence[i] < *x.sequence[i];
        }
    }
    return sequence.size() < x.sequence.size();
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

    std::string tempStr = "";
    for (size_t i=0, j=0; i<tn.size(); i++)
        {
        if ( tn[i] == ' ' )
            tn[i] = '_';
        if ( !(tn[i] == '\'' || tn[i] == '\"') )
            tempStr[j++] = tn[i];
        }
    taxonName = tempStr;
}

