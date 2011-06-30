/**
 * @file
 * This file contains the declaration of CharacterObservation, which is
 * the abstract base class for character observations, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of CharacterObservation
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef CharacterObservationDiscrete_H
#define CharacterObservationDiscrete_H

#include "CharacterObservation.h"
#include <ostream>
#include <vector>



class CharacterObservationDiscrete : public CharacterObservation {

    public:
        virtual bool                            operator==(const CharacterObservation& x) const = 0;          //!< Equality
        virtual bool                            operator!=(const CharacterObservation& x) const = 0;          //!< Inequality

        // Basic utility functions you should not have to override
        virtual void                            printValue(std::ostream& o) const = 0;                        //!< Print value (for user)

        // Basic utility functions you have to override
        virtual CharacterObservationDiscrete*   clone(void) const = 0;                                        //!< Clone object
        virtual const VectorString&             getClass(void) const;                                         //!< Get class vector
        virtual std::string                     richInfo(void) const = 0;                                     //!< Complete info about object

        // Discrete character observation functions
        virtual void                            addState(const char s) = 0;                                   //!< Add a character state to the set of character states
        size_t                                  getNumOnStates(void) const;                                   //!< How many states are observed for the character
        virtual const std::string&              getStateLabels(void) const = 0;                               //!< Get valid state labels
        virtual const char                      getState(void) const = 0;                                     //!< Get the discrete observation
        virtual bool                            isMissAmbig(void) const;                                      //!< Is the character missing or ambiguous
        void                                    setNumStates(size_t n);                                       //!< Set the number of character states
        virtual void                            setState(const char s) = 0;                                   //!< Set the discrete observation

    protected:
                                                CharacterObservationDiscrete(size_t n);                       //!< Constructor taking the number of states as input
        size_t                                  numStates;                                                    //!< The number of discrete states
        std::vector<bool>                       value;                                                        //!< The states, in a binary form to represent ambiguity
};

#endif

