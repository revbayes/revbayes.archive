/**
 * @file
 * This file contains the declaration of NaturalNumbersState, which is
 * the base class for character data types that are represented
 * as natural numbers (such as morphological characters) in RevBayes.
 * Gap states represented as -1
 *
 * @brief Declaration of NaturalNumbersState
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */


#ifndef NaturalNumbersState_H
#define NaturalNumbersState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {
    
    class NaturalNumbersState : public DiscreteCharacterState {
        
    public:
        NaturalNumbersState(void);                                                          //!< Default constructor
        NaturalNumbersState(const std::string &s);                                          //!< Constructor with an observation
        
        bool                            operator==(const CharacterState& x) const;          //!< Equality
        bool                            operator!=(const CharacterState& x) const;          //!< Inequality
        bool                            operator<(const CharacterState& d) const;           //!< Less than
        void                            operator++();                                       //!< Increment
        void                            operator++(int i);                                  //!< Increment
        void                            operator+=(int i);                                  //!< Increment
        void                            operator--();                                       //!< Decrement
        void                            operator--(int i);                                  //!< Decrement
        void                            operator-=(int i);                                  //!< Decrement
        
        NaturalNumbersState*            clone(void) const;                               //!< Get a copy of this object
        
        // Discrete character observation functions
        void                            addState(const std::string &symbol);                //!< Add a character state to the set of character states
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        unsigned int                    getNumberObservedStates(void) const;                //!< How many states are observed for the character
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        size_t                          getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
        unsigned long                   getState(void) const;                               //!< Get the discrete observation
        size_t                          getStateIndex(void) const;
        bool                            isAmbiguous(void) const;                            //!< Is the character missing or ambiguous
        void                            setStateByIndex(size_t index);                      //!< Set the discrete observation
        void                            setState(const std::string &symbol);                //!< Set the discrete observation
//        void                            setState(size_t pos, bool val);                     //!< Set the discrete observation
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
        
        
    private:
		int                             computeState(const std::string &symbol) const;       //!< Compute the internal state value for this character.
        
		int                             state;
        
    };
	extern size_t						g_MAX_NAT_NUM_STATES;
}

#endif