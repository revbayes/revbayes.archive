/**
 * @file
 * This file contains the declaration of RlCharacterState, which is
 * the abstract base class for RlCharacterStates, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of RlCharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RlCharacterState_H
#define RlCharacterState_H

#include "RbLanguageObject.h"

#include <ostream>
#include <string>


class RlCharacterState : public RbLanguageObject {

    public:
        virtual bool                    operator==(const RlCharacterState& x) const = 0;    //!< Equality
        virtual bool                    operator!=(const RlCharacterState& x) const = 0;    //!< Inequality

        // Basic utility functions
        static const std::string&       getClassName(void);                                 //!< Get class name
        static const TypeSpec&          getClassTypeSpec(void);                             //!< Get class type spec

        // Basic utility functions you have to override
        virtual RlCharacterState*       clone(void) const = 0;                              //!< Clone object
        virtual void                    printValue(std::ostream& o) const = 0;              //!< Print value (for user)
    
        virtual const CharacterState&   getCharacterState() const;                          //!< Get the internal character object.

    protected:
                                        RlCharacterState(void);                             //!< Default constructor
};

#endif

