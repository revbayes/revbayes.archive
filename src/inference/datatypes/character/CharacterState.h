/**
 * @file
 * This file contains the declaration of Character, which is
 * the abstract base class for characters, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of Character
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: Character.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef Character_H
#define Character_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class CharacterState {
    
public:
    virtual bool                    operator==(const CharacterState& x) const = 0;      //!< Equality
    virtual bool                    operator!=(const CharacterState& x) const = 0;      //!< Inequality
            
    virtual size_t                  getNumberOfStates(void) const = 0;                  //!< Get the number states for a character (return 0 in base class)
    virtual std::string             getStringValue(void) const = 0;                     //!< Get a representation of the character as a string
    virtual bool                    isGapState(void) const = 0;                         //!< Get whether this is a gapped character state
    virtual bool                    isMissingOrAmbiguous(void) const = 0;               //!< Is the character missing or ambiguous
    virtual void                    setGapState(bool tf) = 0;                           //!< Virtual function call to set whether this is a gapped character

//    virtual double                  getRealValue(void) const { return 0.0; }   //!< Return the double representation of the character
//    virtual unsigned                getUnsignedValue(void) const { return 0; } //!< Get an unsigned representation of the bool set

protected:
    Character(void);                           //!< Default constructor
};

#endif

