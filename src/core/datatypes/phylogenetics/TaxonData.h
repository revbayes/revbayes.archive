/**
 * @file
 * This file contains the declaration of TaxonData, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of TaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef TaxonData_H
#define TaxonData_H

#include "AbstractTaxonData.h"
#include "RbOptions.h"

#include <string>
#include <vector>

namespace RevBayesCore {

    class CharacterState;

    template<class charType>
    class TaxonData : public AbstractTaxonData {
    
    public:
        TaxonData(void);                                                                                            //!< Set type spec of container from type of elements
        TaxonData(const std::string &tname);                                                                        //!< Set type spec of container from type of elements
        TaxonData(const TaxonData<charType> &t);                                                                    //!< Copy constructor
        virtual                                 ~TaxonData();                                                       //!< Destructor
    
        TaxonData&                              operator=(const TaxonData<charType> &t);                            //!< Assignment operator
        charType&                               operator[](size_t i);                                               //!< Index op allowing change
        const charType&                         operator[](size_t i) const;                                         //!< Const index op
        bool                                    operator==(const TaxonData& x) const;                               //!< Less than operator
        bool                                    operator!=(const TaxonData& x) const;                               //!< Less than operator
        bool                                    operator<(const TaxonData& x) const;                                //!< Less than operator
                       
        // TaxonData functions
        void                                    addCharacter(CharacterState *newChar );                             //!< Push back a new character
        void                                    addCharacter(charType *newChar );                                   //!< Push back a new character
        const charType&                         getCharacter(size_t index) const;                                   //!< Get the character at position index
        charType&                               getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        charType&                               getElement(size_t i);                                               //!< Index op allowing change
        const charType&                         getElement(size_t i) const;                                         //!< Const index op
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        void                                    setTaxonName(std::string tn);                                       //!< Set the taxon name
        size_t                                  size(void) const;
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<charType*>                  sequence;
    
    };

    // Global functions using the class
    template<class charType>
    std::ostream&                       operator<<(std::ostream& o, const TaxonData<charType>& x);                                //!< Overloaded output operator

    
}


#include "CharacterState.h"
#include "RbException.h"




/** Constructor with element type, used to properly construct vectors */
template<class charType>
RevBayesCore::TaxonData<charType>::TaxonData(void) : taxonName(""), sequence() {
    
}


/** Constructor with element type, used to properly construct vectors */
template<class charType>
RevBayesCore::TaxonData<charType>::TaxonData(const std::string &tname) : taxonName(tname), sequence() {
    
}



/** Constructor with element type, used to properly construct vectors */
template<class charType>
RevBayesCore::TaxonData<charType>::TaxonData(const TaxonData<charType> &t) : taxonName(t.taxonName), sequence() {
    
    typename std::vector<charType*>::const_iterator it;
    for (it = t.sequence.begin(); it != t.sequence.end(); ++it) 
    {
        charType* theChar = *it;
        sequence.push_back( theChar->clone() );
    }
}


/** Constructor with element type, used to properly construct vectors */
template<class charType>
RevBayesCore::TaxonData<charType>::~TaxonData(void) {
    
    typename std::vector<charType*>::iterator it;
    for (it = sequence.begin(); it != sequence.end(); ++it) 
    {
        charType* theChar = *it;
        delete theChar;
    }
}


/* Assignment operator */
template<class charType>
RevBayesCore::TaxonData<charType>& RevBayesCore::TaxonData<charType>::operator=(const TaxonData<charType> &t) {
    
    if ( &t != this ) 
    {
        // free the current memory
        typename std::vector<charType*>::const_iterator it;
        for (it = sequence.begin(); it != sequence.end(); ++it) 
        {
            const charType* theChar = *it;
            delete theChar;
        }
        
        // clone the characters
        for (it = t.sequence.begin(); it != t.sequence.end(); ++it) 
        {
            charType* theChar = *it;
            sequence.push_back( theChar->clone() );
        }
        
        taxonName = t.taxonName;
    }
    
    return *this;
}


/* Subscript operator */
template<class charType>
charType& RevBayesCore::TaxonData<charType>::operator[](size_t i) {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return *sequence[i];
}


/* Subscript const operator */
template<class charType>
const charType& RevBayesCore::TaxonData<charType>::operator[](size_t i) const {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return *sequence[i];
}


template<class charType>
bool RevBayesCore::TaxonData<charType>::operator==(const TaxonData &x) const {
    
    if ( taxonName != x.taxonName ) 
    {
        return false;
    }
    
    if ( sequence.size() != x.sequence.size() ) 
    {
        return false;
    }
    
    for (size_t i = 0; i < sequence.size(); ++i) 
    {
        if ( *sequence[i] != *x.sequence[i] ) 
        {
            return false;
        }
    }
    
    return true;
}


template<class charType>
bool RevBayesCore::TaxonData<charType>::operator!=(const TaxonData<charType> &x) const {
    
    return !operator==(x);
}


template<class charType>
bool RevBayesCore::TaxonData<charType>::operator<(const TaxonData<charType> &x) const {
    
    for (size_t i = 0; i < sequence.size() && i < x.sequence.size(); ++i) 
    {
        if ( *sequence[i] != *x.sequence[i] ) 
        {
            return *sequence[i] < *x.sequence[i];
        }
    }
    
    return sequence.size() < x.sequence.size();
}

/* Push back a new character */
template<class charType>
void RevBayesCore::TaxonData<charType>::addCharacter( CharacterState *newChar ) {
    
#ifdef ASSERTIONS_ALL
    if ( dynamic_cast<charType*>( newChar ) == NULL ) 
    {
        throw RbException("Inserting wrong character type into TaxonData!!!");
    }
#endif
    
    sequence.push_back( static_cast<charType*>(newChar) );
    
}

/* Push back a new character */
template<class charType>
void RevBayesCore::TaxonData<charType>::addCharacter( charType *newChar ) {
    
    sequence.push_back( newChar );
    
}


template<class charType>
charType& RevBayesCore::TaxonData<charType>::getCharacter(size_t index) {
    
    return operator[](index);
}


template<class charType>
const charType& RevBayesCore::TaxonData<charType>::getCharacter(size_t index) const {
    
    return operator[](index);
}


/* Subscript operator */
template<class charType>
charType& RevBayesCore::TaxonData<charType>::getElement(size_t i) {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return *sequence[i];
}


/* Subscript const operator */
template<class charType>
const charType& RevBayesCore::TaxonData<charType>::getElement(size_t i) const {
    
    if (i >= sequence.size())
        throw RbException("Index out of bounds");
    
    return *sequence[i];
}


template<class charType>
size_t RevBayesCore::TaxonData<charType>::getNumberOfCharacters(void) const {
    
    return sequence.size();
}


template<class charType>
const std::string& RevBayesCore::TaxonData<charType>::getTaxonName(void) const {
    
    return taxonName;
}



template<class charType>
void RevBayesCore::TaxonData<charType>::setTaxonName(std::string tn) {
    
    taxonName = tn;
}


template<class charType>
size_t RevBayesCore::TaxonData<charType>::size(void) const {
    
    return sequence.size();
}


template<class charType>
std::ostream& RevBayesCore::operator<<(std::ostream& o, const TaxonData<charType>& x) {
    
    o << x.getTaxonName() << ":" << std::endl;
    for (size_t i = 0; i < x.getNumberOfCharacters(); ++i) 
    {
        o << x[i];
    }
    o << std::endl;
    
    return o;
}


#endif
