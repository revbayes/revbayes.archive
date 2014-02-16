/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterData_H
#define CharacterData_H

#include "AbstractCharacterData.h"
#include "CharacterState.h"
#include "MatrixReal.h"
#include "TaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class charType>
    class CharacterData : public AbstractCharacterData {
        
    public:
        CharacterData();                                                            //!< Constructor requires character type
        CharacterData(const CharacterData& x);                                      //!< Copy constructor to deal with sequenceTypeRule
        virtual                            ~CharacterData(void);                                                        //!< Destructor to deal with sequenceTypeRule
        
        // Overloaded operators
        CharacterData&                      operator=(const CharacterData& x);                                          //!< Assignment operator
        const TaxonData<charType>&          operator[](size_t i) const;                                                 //!< Subscript operator (const)
        bool                                operator==(const CharacterData& x) const;                                   //!< Less than operator
        bool                                operator!=(const CharacterData& x) const;                                   //!< Less than operator
        bool                                operator<(const CharacterData& x) const;                                    //!< Less than operator
        
        // implemented methods of the Cloneable interface
        CharacterData<charType>*            clone(void) const;
        
        // Container functions
        void                                clear(void);
        
        // CharacterData functions
        void                                addTaxonData(const AbstractTaxonData &obs);                                 //!< Add taxon data
        void                                addTaxonData(const AbstractTaxonData &obs, bool forceAdd);                  //!< Add taxon data
        void                                addTaxonData(const TaxonData<charType> &obs);                               //!< Add taxon data
        void                                addTaxonData(const TaxonData<charType> &obs, bool forceAdd);                //!< Add taxon data
//        MatrixReal                          computeStateFrequencies(void);
        void                                excludeCharacter(size_t i);                                                 //!< Exclude character
        void                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                excludeTaxon(std::string& s);                                               //!< Exclude taxon
        const charType&                     getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                         getDatatype(void) const;
        const std::string&                  getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        bool                                getIsHomologyEstablished(void) const;                                       //!< Returns whether the homology of the characters has been established
        size_t                              getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                              getNumberOfCharacters(size_t idx) const;                                    //!< Number of characters for a specific taxon
        size_t                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        size_t                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        TaxonData<charType>&                getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const TaxonData<charType>&          getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        TaxonData<charType>&                getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const TaxonData<charType>&          getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        const std::vector<std::string>&     getTaxonNames(void) const;                                                  //!< Get the names of the taxa
        const std::string&                  getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                isTaxonExcluded(std::string& s) const;                                      //!< Is the taxon excluded
        void                                restoreCharacter(size_t i);                                                 //!< Restore character
        void                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                restoreTaxon(std::string& s);                                               //!< Restore taxon
        void                                setFileName(const std::string &fn);                                         //!< Set the file name
        void                                setIsHomologyEstablished(bool tf);                                          //!< Set whether the homology of the characters has been established
        
    protected:
        // Utility functions
        size_t                              indexOfTaxonWithName(std::string& s) const;                                 //!< Get the index of the taxon
        bool                                isCharacterConstant(size_t idx) const;                                      //!< Is the idx-th character a constant pattern?
        bool                                isCharacterMissingOrAmbiguous(size_t idx) const;                            //!< Does the character have missing or ambiguous data?
        size_t                              numConstantPatterns(void) const;                                            //!< The number of constant patterns
        size_t                              numMissAmbig(void) const;                                                   //!< The number of patterns with missing or ambiguous characters
        
        // Member variables
        std::set<size_t>                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::set<size_t>                    deletedCharacters;                                                          //!< Set of deleted characters
        std::string                         fileName;                                                                   //!< The path/filename from where this matrix originated
        std::vector<std::string>            sequenceNames;                                                              //!< names of the sequences
        size_t                              sequenceLength;                                                             //!< The length of each sequence
        bool                                isHomologyEstablished;                                                      //!< Whether the homology of the characters has been established
        
        std::map<std::string, TaxonData<charType> >    taxonMap;
        
    };
    
    
}


#include "CharacterState.h"
#include "CharacterData.h"
#include "RbException.h"
#include "TaxonData.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/* Constructor */
template<class charType>
RevBayesCore::CharacterData<charType>::CharacterData() {
}


/* Copy constructor */
template<class charType>
RevBayesCore::CharacterData<charType>::CharacterData(const CharacterData& x) : sequenceNames( x.sequenceNames ) {
    
    deletedTaxa             = x.deletedTaxa;
    deletedCharacters       = x.deletedCharacters;
    fileName                = x.fileName;
    isHomologyEstablished   = x.isHomologyEstablished;
    taxonMap                = x.taxonMap;
    
}


/* Destructor */
template<class charType>
RevBayesCore::CharacterData<charType>::~CharacterData( void ) {
    
    taxonMap.clear();
    
}


/* Assignment operator */
template<class charType>
RevBayesCore::CharacterData<charType>& RevBayesCore::CharacterData<charType>::operator=( const CharacterData& x ) {
    
    if ( this != &x ) 
    {
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
template<class charType>
const RevBayesCore::TaxonData<charType>& RevBayesCore::CharacterData<charType>::operator[]( const size_t i ) const {
    
    return getTaxonData( i );
}


template<class charType>
bool RevBayesCore::CharacterData<charType>::operator==(const CharacterData<charType> &x) const {
    
    
    if ( deletedTaxa != x.deletedTaxa ) 
    {
        return false;
    }
    
    if ( deletedCharacters != x.deletedCharacters ) 
    {
        return false;
    }
    
    if ( fileName != x.fileName ) 
    {
        return false;
    }
    
    if ( sequenceNames != x.sequenceNames ) 
    {
        return false;
    }
    
    if ( sequenceLength != x.sequenceLength ) 
    {
        return false;
    }
    
    if ( isHomologyEstablished != x.isHomologyEstablished ) 
    {
        return false;
    }
    
    if ( taxonMap != x.taxonMap ) 
    {
        return false;
    }
    
    return true;
}


template<class charType>
bool RevBayesCore::CharacterData<charType>::operator!=(const CharacterData<charType> &x) const {
    
    return !operator==(x);
}


template<class charType>
bool RevBayesCore::CharacterData<charType>::operator<(const CharacterData<charType> &x) const {
    
    return sequenceNames.size() < x.sequenceNames.size();
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
template<class charType>
void RevBayesCore::CharacterData<charType>::addTaxonData(const AbstractTaxonData &obs, bool forceAdd) {
    
#ifdef ASSERTIONS_ALL
    if ( dynamic_cast<const TaxonData<charType>* >( &obs ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into CharacterData!!!");
    }
#endif
    
    // add the sequence name to the list
    sequenceNames.push_back( obs.getTaxonName() );
    
    // add the sequence also as a member so that we can access it by name
    taxonMap.insert( std::pair<std::string, TaxonData<charType> >( obs.getTaxonName(), static_cast<const TaxonData<charType>& >( obs ) ) );
    
}

template<class charType>
void RevBayesCore::CharacterData<charType>::addTaxonData(const AbstractTaxonData &obs ) {
    
    // set the number of character per sequence
    if ( getNumberOfTaxa() > 0 && obs.getNumberOfCharacters() != getNumberOfCharacters() ) 
    {
        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs.getNumberOfCharacters() << " to aligned character matrix of length " << getNumberOfCharacters();
        throw RbException( msg );
    }
    
    addTaxonData(obs, true);
    
}


/** Add a sequence to the character matrix. For now, we require same data type and same length. */
template<class charType>
void RevBayesCore::CharacterData<charType>::addTaxonData(const TaxonData<charType> &obs, bool forceAdd) {
    
    // add the sequence name to the list
    sequenceNames.push_back( obs.getTaxonName() );
    
    // add the sequence also as a member so that we can access it by name
    taxonMap.insert( std::pair<std::string, TaxonData<charType> >( obs.getTaxonName(), obs ) );
    
}

template<class charType>
void RevBayesCore::CharacterData<charType>::addTaxonData(const TaxonData<charType> &obs ) {
    
    // set the number of character per sequence
    if ( getNumberOfTaxa() > 0 && obs.getNumberOfCharacters() != getNumberOfCharacters() ) 
    {
        std::ostringstream msg;
        msg << "Invalid attempt to add sequence of length " << obs.getNumberOfCharacters() << " to aligned character matrix of length " << getNumberOfCharacters();
        throw RbException( msg );
    }
    
    addTaxonData(obs, true);
    
}


/** clear the object */
template<class charType>
void RevBayesCore::CharacterData<charType>::clear( void ) {
    
    sequenceNames.clear();
    taxonMap.clear();
    
}


/** clone the object */
template<class charType>
RevBayesCore::CharacterData<charType>* RevBayesCore::CharacterData<charType>::clone( void ) const {
    
    return new CharacterData<charType>(*this);
}


/** Exclude a character */
template<class charType>
void RevBayesCore::CharacterData<charType>::excludeCharacter(size_t i) {
    
    if (i < 1 || i > getTaxonData( 0 ).size() ) 
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.insert( i-1 );
    
}


/** Exclude a taxon */
template<class charType>
void RevBayesCore::CharacterData<charType>::excludeTaxon(size_t i) {
    
    if (i >= taxonMap.size()) 
    {
        std::stringstream o;
        o << "Only " << taxonMap.size() << " taxa in matrix";
        throw RbException( o.str() );
    }
    
    deletedTaxa.insert( i );
}


/** Exclude a taxon */
template<class charType>
void RevBayesCore::CharacterData<charType>::excludeTaxon(std::string& s) {
    
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
template<class charType>
const charType& RevBayesCore::CharacterData<charType>::getCharacter( size_t tn, size_t cn ) const {
    
    if ( cn < 1 || cn > getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    return getTaxonData( tn )[cn-1];
}


template<class charType>
std::string RevBayesCore::CharacterData<charType>::getDatatype(void) const {
    
    std::string dt = "";
    if ( sequenceNames.size() > 0 ) 
    {
        const TaxonData<charType> &t = getTaxonData( sequenceNames[0] );
        if ( t.size() > 0 ) 
        {
            dt = t[0].getDatatype();
        }
        
    }
    
    return dt;
}


template<class charType>
const std::string& RevBayesCore::CharacterData<charType>::getFileName(void) const {
    
    return fileName;
}


template<class charType>
bool RevBayesCore::CharacterData<charType>::getIsHomologyEstablished(void) const {
    
    return isHomologyEstablished;
}



/** Return the number of characters in each vector of taxon observations */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::getNumberOfCharacters(void) const {
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(0).getNumberOfCharacters() - deletedCharacters.size();
    }
    return 0;
}


/** Return the number of characters in each vector of taxon observations */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::getNumberOfCharacters(size_t idx) const {
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(idx).getNumberOfCharacters() - deletedCharacters.size();
    }
    
    return 0;
}


/** Get the number of states for the characters in this matrix. We
 assume that all of the characters in the matrix are of the same
 type and have the same number of potential states. */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::getNumberOfStates(void) const {
    
    // Get the first character in the matrix
    if ( getNumberOfTaxa() == 0 )
        return 0;
    
    const TaxonData<charType>& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
        return 0;
    
    return sequence[0].getNumberOfStates();
}


template<class charType>
size_t RevBayesCore::CharacterData<charType>::getNumberOfTaxa(void) const {
    
    return sequenceNames.size();
}


/** Get sequence with index tn */
template<class charType>
const RevBayesCore::TaxonData<charType>& RevBayesCore::CharacterData<charType>::getTaxonData( size_t tn ) const {
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, TaxonData<charType> >::const_iterator& i = taxonMap.find( name ); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/** Get sequence with index tn */
template<class charType>
RevBayesCore::TaxonData<charType>& RevBayesCore::CharacterData<charType>::getTaxonData( size_t tn ) {
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, TaxonData<charType> >::iterator& i = taxonMap.find( name ); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}



/** Get sequence with name tn */
template<class charType>
const RevBayesCore::TaxonData<charType>& RevBayesCore::CharacterData<charType>::getTaxonData( const std::string &tn ) const {
    
    const typename std::map<std::string, TaxonData<charType> >::const_iterator& i = taxonMap.find(tn); 
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
    }
    else 
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/** Get sequence with name tn */
template<class charType>
RevBayesCore::TaxonData<charType>& RevBayesCore::CharacterData<charType>::getTaxonData( const std::string &tn ) {
    
    const typename std::map<std::string, TaxonData<charType> >::iterator& i = taxonMap.find(tn); 
    
    if ( tn == "" ) 
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    if (i != taxonMap.end() ) 
    {
        return i->second;
    }
    else 
    {
        
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


template<class charType>
const std::vector<std::string>& RevBayesCore::CharacterData<charType>::getTaxonNames( void ) const {
    
    return sequenceNames;
}



/** Get taxon with index idx */
template<class charType>
const std::string& RevBayesCore::CharacterData<charType>::getTaxonNameWithIndex( size_t idx ) const {
    
    return sequenceNames[idx];
}



/** Return the index of the element ( the index of the taxon with name elemName ) */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::indexOfTaxonWithName( std::string& s ) const {
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++) 
    {
        if (s == sequenceNames[i] ) 
        {
            return i;
        }
    }
    
    return -1;
}


/** Is this character pattern constant? */
template<class charType>
bool RevBayesCore::CharacterData<charType>::isCharacterConstant(size_t idx) const {
    
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
template<class charType>
bool RevBayesCore::CharacterData<charType>::isCharacterExcluded(size_t i) const {
    
	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
		return true;
    
    return false;
}


/** Does the character have missing or ambiguous characters */
template<class charType>
bool RevBayesCore::CharacterData<charType>::isCharacterMissingOrAmbiguous(size_t idx) const {
    
    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
    {
        if ( isTaxonExcluded(i) == false )
        {
            const CharacterState& c = getCharacter( i, idx );
            if ( c.isGapState() == true || c.isAmbiguous() == true )
                return true;
        }
    }
    
    return false;
}


/** Is the taxon excluded */
template<class charType>
bool RevBayesCore::CharacterData<charType>::isTaxonExcluded(size_t i) const {
    
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    
    return false;
}


/** Is the taxon excluded */
template<class charType>
bool RevBayesCore::CharacterData<charType>::isTaxonExcluded(std::string& s) const {
    
    size_t i = indexOfTaxonWithName(s);
	std::set<size_t>::const_iterator it = deletedTaxa.find( i );
	if ( it != deletedTaxa.end() )
		return true;
    
    return false;
}


/** Calculates and returns the number of constant characters */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::numConstantPatterns(void) const {
    
    size_t nc = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
    {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
    }
    
    return nc;
}


/** Returns the number of characters with missing or ambiguous data */
template<class charType>
size_t RevBayesCore::CharacterData<charType>::numMissAmbig(void) const {
    
    size_t nma = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
    {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
    }
    
    return nma;
}


/** Restore a character */
template<class charType>
void RevBayesCore::CharacterData<charType>::restoreCharacter(size_t i) {
    
    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    deletedCharacters.erase( i );
    
}


/** Restore a taxon */
template<class charType>
void RevBayesCore::CharacterData<charType>::restoreTaxon(size_t i) {
    
    if ( i >= getNumberOfTaxa() )
        return;
    
    deletedTaxa.erase( i );
    
}


/** Restore a taxon */
template<class charType>
void RevBayesCore::CharacterData<charType>::restoreTaxon(std::string& s) {
    
    size_t i = indexOfTaxonWithName( s );
    
    deletedTaxa.erase( i );
    
}


template<class charType>
void RevBayesCore::CharacterData<charType>::setFileName(const std::string& fn) {
    
    fileName = fn;
    
}


template<class charType>
void RevBayesCore::CharacterData<charType>::setIsHomologyEstablished(bool tf) {
    
    isHomologyEstablished = tf;
    
}



#endif

