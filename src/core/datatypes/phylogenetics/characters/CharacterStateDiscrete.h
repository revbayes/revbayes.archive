/**
 * @file
 * This file contains the declaration of CharacterStateDiscrete, which is
 * the abstract base class for character observations, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of CharacterStateDiscrete
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef CharacterStateDiscrete_H
#define CharacterStateDiscrete_H

#include "Character.h"
#include <ostream>
#include <vector>


class CharacterStateDiscrete : public Character {

    public:
        virtual bool                            operator==(const Character& x) const = 0;           //!< Equality
        virtual bool                            operator!=(const Character& x) const = 0;           //!< Inequality

        // Basic utility functions you can override
        virtual const MemberRules&              getMemberRules(void) const;                         //!< Get member rules
        virtual void                            initialize(const Vector& attributes);                     //!< Initialize this object with the values inside the vector

        // Basic utility functions you have to override
        virtual CharacterStateDiscrete*         clone(void) const = 0;                              //!< Clone object
        static const std::string&               getClassName(void);                                 //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                             //!< Get class type spec
        virtual void                            printValue(std::ostream& o) const = 0;              //!< Print value (for user)

        // Discrete character observation functions
        virtual void                            addState(const char s) = 0;                         //!< Add a character state to the set of character states
        bool                                    getIsGapState(void) const { return isGapState; }    //!< Get whether this is a gapped character state
        size_t                                  getNumberObservedStates(void) const;                //!< How many states are observed for the character
        virtual const std::string&              getStateLabels(void) const = 0;                     //!< Get valid state labels
        virtual size_t                          getNumberOfStates(void) const { return numStates; } //!< Get the number of discrete states for the character
        virtual const char                      getState(void) const = 0;                           //!< Get the discrete observation
        const std::vector<bool>&                getStateVector(void) const { return value; }        //!< Get the raw vector of states
        virtual unsigned                        getUnsignedValue(void) const;                       //!< Get an unsigned reprsentation of the bool set
        virtual bool                            isMissingOrAmbiguous(void) const;                   //!< Is the character missing or ambiguous
        void                                    setNumberOfStates(size_t n);                        //!< Set the number of character states
        virtual void                            setState(const char s) = 0;                         //!< Set the discrete observation
        void                                    setValue(int x);                                    //!< Set value from int
        void                                    setValue(const std::vector<bool> &v);               //!< Set the value vector
        void                                    setIsGapState(bool tf) { isGapState = tf; }         //!< Set whether this is a gapped character

    protected:
                                                CharacterStateDiscrete(size_t n);                  //!< Constructor taking the number of states as input
        size_t                                  numStates;                                         //!< The number of discrete states
        std::vector<bool>                       value;                                             //!< The states, in a binary form to represent ambiguity
        bool                                    isGapState;                                        //!< Is this a gapped character state?
};

#endif

