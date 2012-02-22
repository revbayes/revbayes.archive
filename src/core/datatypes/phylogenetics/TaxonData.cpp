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
#include "TaxonData.h"


/** Constructor with element type, used to properly construct vectors */
TaxonData::TaxonData(const std::string& elemType, const std::string tname) : Vector(elemType), taxonName(tname) {
    
}


/** Subscript operator */
Character& TaxonData::operator[](size_t i) {
    
    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *static_cast<Character*>( elements[i] );
}


/** Subscript const operator */
const Character& TaxonData::operator[](size_t i) const {
    
    if (i >= elements.size())
        throw RbException("Index out of bounds");
    return *static_cast<const Character*>( elements[i] );
}

/** Push back a new character */
void TaxonData::addCharacter( Character* newChar ) {
    
    if ( newChar == NULL || !newChar->isTypeSpec( elementType ) )
        throw RbException( "Inappropriate character type" );
    
    elements.push_back( newChar );
}


TaxonData* TaxonData::clone(void) const {

    return new TaxonData(*this);
}


/** Get class name of object */
const std::string& TaxonData::getClassName(void) { 
    
    static std::string rbClassName = "Taxon data";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& TaxonData::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Vector::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& TaxonData::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


const Character& TaxonData::getCharacter(size_t index) const {

    return operator[](index);
}


size_t TaxonData::getNumberOfCharacters(void) const {

    return elements.size();
}


const std::string& TaxonData::getTaxonName(void) const {

    return taxonName;
}


void TaxonData::printValue(std::ostream &o) const {

    o << taxonName << ":" << std::endl;
    Vector::printValue(o);
}



void TaxonData::setTaxonName(std::string tn) {

    taxonName = tn;
}

