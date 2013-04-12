/**
 * @file
 * This file contains the declaration of AbstractTaxonData, an
 * abstract base class for different types of character vectors.
 *
 * @brief Declaration of AbstractTaxonData
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-10 14:09:12 +0200 (Tue, 10 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: AbstractTaxonData.h 1660 2012-07-10 12:09:12Z hoehna $
 */

#ifndef AbstractTaxonData_H
#define AbstractTaxonData_H

#include <string>
#include <vector>

namespace RevBayesCore {
    
    class CharacterState;

    class AbstractTaxonData {
    
    public:
        virtual                                ~AbstractTaxonData(void) {}
    
        virtual CharacterState&                 operator[](size_t i) = 0;                                           //!< Index op allowing change
        virtual const CharacterState&           operator[](size_t i) const = 0;                                     //!< Const index op
    
        // AbstractTaxonData functions
        virtual void                            addCharacter(CharacterState *newChar ) = 0;                         //!< Push back a new character
        virtual const CharacterState&           getCharacter(size_t index) const = 0;                               //!< Get the character at position index
        virtual CharacterState&                 getCharacter(size_t index) = 0;                                     //!< Get the character at position index (non-const to return non-const character)
        virtual size_t                          getNumberOfCharacters(void) const = 0;                              //!< How many characters
        virtual const std::string&              getTaxonName(void) const = 0;                                       //!< Return the name of the character vector
        virtual void                            setTaxonName(std::string tn) = 0;                                   //!< Set the taxon name
    
    protected:
        AbstractTaxonData() {}                                                                                      //!< Default constructor

    };

}

#endif
