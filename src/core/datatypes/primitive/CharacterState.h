/**
 * @file
 * This file contains the declaration of CharacterState, which is
 * the abstract base class for character state data types in
 * RevBayes.
 *
 * @brief Declaration of CharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterState_H
#define CharacterState_H

#include "Categorical.h"

#include <ostream>
#include <string>


const std::string CharacterState_name = "Character State";

class CharacterState : public Categorical {

    public:
        // Basic utility functions you have to override
        virtual CharacterState*         clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class vector
        virtual std::string             richInfo(void) const = 0;                                           //!< Complete info about object

        // Categorical functions you have to override
        virtual const std::string&      getStateLabels(void) const = 0;                                     //!< Get valid state labels
        virtual const std::string&      getNALabels(void) const = 0;                                        //!< Get NA labels corresponding to value = -1, -2, etc

    protected:
                                        CharacterState(void);                                               //!< Default constructor
                                        CharacterState(int x);                                              //!< Constructor from int value
                                        CharacterState(char c);                                             //!< Constructor from char value (state code or symbol)
};

#endif

