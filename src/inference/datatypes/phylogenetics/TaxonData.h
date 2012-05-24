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

#include "MemberObject.h"
#include "RlVector.h"

#include <ostream>
#include <string>
#include <vector>

class CharacterState;


class TaxonData {
    
    public:
                                                TaxonData(const std::string tname="");                              //!< Set type spec of container from type of elements
        
        CharacterState&                         operator[](size_t i);                                               //!< Index op allowing change
        const CharacterState&                   operator[](size_t i) const;                                         //!< Const index op
                       
        // TaxonData functions
        void                                    addCharacter(const CharacterState &newChar );                       //!< Push back a new character
        const CharacterState&                   getCharacter(size_t index) const;                                   //!< Get the character at position index
        CharacterState&                         getCharacter(size_t index);                                         //!< Get the character at position index (non-const to return non-const character)
        size_t                                  getNumberOfCharacters(void) const;                                  //!< How many characters
        const std::string&                      getTaxonName(void) const;                                           //!< Return the name of the character vector
        void                                    setTaxonName(std::string tn);                                       //!< Set the taxon name
 
        
    private:
        std::string                             taxonName;                                                          //!< Name of the taxon for this vector of characters               
        std::vector<CharacterState>             sequence;
    

};

#endif