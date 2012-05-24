/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Implementation of Alignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Mcmc
 * @package datatypes
 *
 * $Id$
 */

#include "CharacterState.h"
#include "CharacterData.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TaxonData.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/** Constructor requires character type; passes member rules to base class */
CharacterData::CharacterData( const std::string& charType )  
{
    characterType = charType;
}


/** Copy constructor */
CharacterData::CharacterData(const CharacterData& x) : 
sequenceNames( x.sequenceNames )
{

    characterType           = x.characterType;
    deletedTaxa             = x.deletedTaxa;
    deletedCharacters       = x.deletedCharacters;
    fileName                = x.fileName;
    isHomologyEstablished   = x.isHomologyEstablished;
    taxonMap                = x.taxonMap;
}


/** Destructor */
CharacterData::~CharacterData( void ) {

}


/** Assignment operator */
CharacterData& CharacterData::operator=( const CharacterData& x ) {

    if ( this != &x ) 
        {
        if ( characterType != x.characterType )
            throw RbException( "Invalid assignment of character matrices: sequence data types differ" );

        characterType           = x.characterType;
        deletedTaxa             = x.deletedTaxa;
        deletedCharacters       = x.deletedCharacters;
        fileName                = x.fileName;
        isHomologyEstablished   = x.isHomologyEstablished;
        taxonMap                = x.taxonMap;
        sequenceNames           = x.sequenceNames;
        }
    return (*this);
}


/** Index (const) operator */
const TaxonData& CharacterData::operator[]( const size_t i ) const {

    return getTaxonData( i );
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
void CharacterData::addTaxonData(const TaxonData &obs, bool forceAdd) {
    
    // add the sequence name to the list
    sequenceNames.push_back( obs.getTaxonName() );
    
    // add the sequence also as a member so that we can access it by name
    taxonMap.insert( std::pair<std::string, TaxonData>( obs.getTaxonName(), obs ) );
}

void CharacterData::addTaxonData(const  TaxonData &obs ) {

    // set the number of character per sequence
    if ( getNumberOfTaxa() > 0 && obs.getNumberOfCharacters() != getNumberOfCharacters() ) 
        {
        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs.getNumberOfCharacters() << " to aligned character matrix of length " << getNumberOfCharacters();
        throw RbException( msg );
        }
        
    addTaxonData(obs, true);
}


/** clear the oblect */
void CharacterData::clear( void ) {
    
    sequenceNames.clear();
    taxonMap.clear();
}


/** Exclude a character */
void CharacterData::excludeCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Only " + RbString(int(getNumberOfCharacters())) + " characters in matrix" );

    deletedCharacters.insert( i );
}


/** Exclude a taxon */
void CharacterData::excludeTaxon(size_t i) {

    if (i >= taxonMap.size())
        throw RbException( "Only " + RbString(int(taxonMap.size())) + " taxa in matrix" );

    deletedTaxa.insert( i );
}


/** Exclude a taxon */
void CharacterData::excludeTaxon(std::string& s) {

    for (size_t i = 0; i < getNumberOfTaxa(); i++) 
        {
        if (s == sequenceNames[i] ) 
            {
            deletedTaxa.insert( i );
            break;
            }
        }
}



/** Return a pointer to a character element in the character matrix */
const CharacterState& CharacterData::getCharacter( size_t tn, size_t cn ) const {

    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    return getTaxonData( tn )[cn];
}



const std::string& CharacterData::getDataType(void) const {

    return characterType;
}


const std::string& CharacterData::getFileName(void) const {

    return fileName;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterData::getNumberOfCharacters(void) const {

    if (getNumberOfTaxa() > 0) 
        {
        return getTaxonData(0).getNumberOfCharacters();
        }
    return 0;
}


/** Return the number of characters in each vector of taxon observations */
size_t CharacterData::getNumberOfCharacters(size_t idx) const {

    if (getNumberOfTaxa() > 0) 
        {
        return getTaxonData(idx).getNumberOfCharacters();
        }
    return 0;
}


/** Get the number of states for the characters in this matrix. We
    assume that all of the characters in the matrix are of the same
    type and have the same number of potential states. */
size_t CharacterData::getNumberOfStates(void) const {

    // Get the first character in the matrix
    if ( getNumberOfTaxa() == 0 )
        return 0;

    const TaxonData& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
        return 0;

    return sequence[0].getNumberOfStates();
}


size_t CharacterData::getNumberOfTaxa(void) const {
    return sequenceNames.size();
}


/** Get sequence with index tn */
const TaxonData& CharacterData::getTaxonData( size_t tn ) const {

    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );

    const std::string& name = static_cast<const RbString &>( sequenceNames[tn] ).getValue();
    const std::map<std::string, TaxonData>::const_iterator& i = taxonMap.find(name); 
    
    if (i != taxonMap.end() ) {
        return i->second;
    }
    else {
        throw RbException("Cannot find the taxon with name '" + name + "' in the CharacterData matrix. This should actually never happen. Please report this bug!");
    }

}


/** Get sequence with index tn */
TaxonData& CharacterData::getTaxonData( size_t tn ) {
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = static_cast<const RbString &>( sequenceNames[tn] ).getValue();
    const std::map<std::string, TaxonData>::iterator& i = taxonMap.find(name); 
    
    if (i != taxonMap.end() ) {
        return i->second;
    }
    else {
        throw RbException("Cannot find the taxon with name '" + name + "' in the CharacterData matrix. This should actually never happen. Please report this bug!");
    }
    
}


/** Get taxon with index idx */
const std::string& CharacterData::getTaxonNameWithIndex( size_t idx ) const {

    return sequenceNames[idx];
}


/** Return the index of the element ( the index of the taxon with name elemName ) */
size_t CharacterData::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) 
        {
        if (s == static_cast<const RbString &>( sequenceNames[i] ).getValue() ) 
            {
            return i;
            }
        }
    return -1;
}


/** Is this character pattern constant? */
bool CharacterData::isCharacterConstant(size_t idx) const {

    const CharacterState* f = NULL;
    for ( size_t i=0; i<getNumberOfTaxa(); i++ ) 
        {
        if ( isTaxonExcluded(i) == false ) 
            {
            if ( f == NULL )
                f = &getCharacter( i, idx );
            else 
                {
                const CharacterState* s = &getCharacter( i , idx );
                if ( (*f) != (*s) )
                    return false;
                }
            }
        }
    return true;
}


/** Is the character excluded */
bool CharacterData::isCharacterExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    return false;
}


/** Does the character have missing or ambiguous characters */
bool CharacterData::isCharacterMissingOrAmbiguous(size_t idx) const {

    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
        {
        if ( isTaxonExcluded(i) == false )
            {
            const CharacterState& c = getCharacter( i, idx );
            if ( c.isMissingOrAmbiguous() == true )
                return true;
            }
        }
    return false;
}


/** Is the taxon excluded */
bool CharacterData::isTaxonExcluded(size_t i) const {

	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Is the taxon excluded */
bool CharacterData::isTaxonExcluded(std::string& s) const {

    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    return false;
}


/** Calculates and returns the number of constant characters */
size_t CharacterData::numConstantPatterns(void) const {

    size_t nc = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
        }
    return nc;
}


/** Returns the number of characters with missing or ambiguous data */
size_t CharacterData::numMissAmbig(void) const {

    size_t nma = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
        {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
        }
    return nma;
}


/** Restore a character */
void CharacterData::restoreCharacter(size_t i) {

    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );

    deletedCharacters.erase( i );
}


/** Restore a taxon */
void CharacterData::restoreTaxon(size_t i) {

    if ( i >= getNumberOfTaxa() )
        return;

    deletedTaxa.erase( i );
}


/** Restore a taxon */
void CharacterData::restoreTaxon(std::string& s) {

    size_t i = indexOfTaxonWithName( s );

    deletedTaxa.erase( i );
}




