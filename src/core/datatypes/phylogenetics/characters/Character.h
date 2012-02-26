/**
 * @file
 * This file contains the declaration of Character, which is
 * the abstract base class for characters, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of Character
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef Character_H
#define Character_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class Character : public RbLanguageObject {

    public:
        virtual bool                    operator==(const Character& x) const = 0;  //!< Equality
        virtual bool                    operator!=(const Character& x) const = 0;  //!< Inequality

        // Basic utility functions you should not have to override
        virtual void                    printValue(std::ostream& o) const = 0;     //!< Print value (for user)

        // Basic utility functions you have to override
        virtual Character*              clone(void) const = 0;                     //!< Clone object
        static const std::string&       getClassName(void);                        //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                    //!< Get class type spec
        
        virtual bool                    getIsGapState(void) const = 0;             //!< Get whether this is a gapped character state
        virtual size_t                  getNumberOfStates(void) const { return 0; }//!< Get the number states for a character (return 0 in base class)
        virtual double                  getRealValue(void) const { return 0.0; }   //!< Return the double representation of the character
        virtual unsigned                getUnsignedValue(void) const { return 0; } //!< Get an unsigned representation of the bool set
        virtual std::string             getStringValue(void) const { return ""; }  //!< Get a representation of the character as a string
        virtual bool                    isMissingOrAmbiguous(void) const = 0;      //!< Is the character missing or ambiguous
        virtual void                    setValue(double x) {};                     //!< Virtual function call to enable CharacterContinuous to set data observation
        virtual void                    setIsGapState(bool tf) = 0;                //!< Virtual function call to set whether this is a gapped character

    protected:
                                        Character(void);                           //!< Default constructor
};

#endif

