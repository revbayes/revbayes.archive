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

#include "RbObject.h"
#include <ostream>



class Character : public RbObject {

    public:
        virtual bool                    operator==(const Character& x) const = 0;  //!< Equality
        virtual bool                    operator!=(const Character& x) const = 0;  //!< Inequality

        // Basic utility functions you should not have to override
        virtual void                    printValue(std::ostream& o) const = 0;     //!< Print value (for user)

        // Basic utility functions you have to override
        virtual Character*              clone(void) const = 0;                     //!< Clone object
        virtual const VectorString&     getClass(void) const;                      //!< Get class vector
        virtual std::string             richInfo(void) const = 0;                  //!< Complete info about object
        
        virtual size_t                  getNumStates(void) { return 0; }           //!< Get the number states for a character (return 0 in base class)
        virtual double                  getRealValue(void) { return 0.0; }         //!< Return the double representation of the character
        virtual unsigned                getUnsignedValue(void) { return 0; }       //!< Get an unsigned representation of the bool set
        virtual bool                    isMissAmbig(void) const = 0;               //!< Is the character missing or ambiguous

    protected:
                                        Character(void);                           //!< Default constructor
};

#endif

