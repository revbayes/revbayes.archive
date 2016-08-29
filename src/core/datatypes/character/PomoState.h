/**
 * @file
 * This file contains the declaration of PomoState, which is
 * the class for the DNA data types plus two-state polymorphic states in RevBayes.
 * Instead of the usual coding by bitfields, we choose coding by ints.
 *
 * @brief Declaration of PomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: PomoState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef PomoState_H
#define PomoState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {
    
    class PomoState : public DiscreteCharacterState {
        
    public:
        PomoState(void);                                                                    //!< Default constructor
        PomoState(unsigned int vps);                                                        //!< Constructor with virtual population size
        PomoState(const std::string &s);                                                    //!< Constructor with an observation
        
        bool                            operator==(const CharacterState& x) const;          //!< Equality
        bool                            operator!=(const CharacterState& x) const;          //!< Inequality
        bool                            operator<(const CharacterState& d) const;           //!< Less than
        void                            operator++();                                       //!< Increment
        void                            operator++(int i);                                  //!< Increment
        void                            operator+=(int i);                                  //!< Increment
        void                            operator--();                                       //!< Decrement
        void                            operator--(int i);                                  //!< Decrement
        void                            operator-=(int i);                                  //!< Decrement
        
        PomoState*                       clone(void) const;                                  //!< Get a copy of this object
        
        // Discrete character observation functions
        void                            addState(const std::string &symbol);                //!< Add a character state to the set of character states
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        unsigned int                    getNumberObservedStates(void) const;                //!< How many states are observed for the character
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        size_t                          getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
        unsigned long                   getState(void) const;                               //!< Get the discrete observation
        size_t                          getStateIndex(void) const;
        void                            setStateByIndex(size_t index);                      //!< Set the discrete observation
        void                            setState(const std::string &symbol);                //!< Set the discrete observation
//      void                            setState(size_t pos, bool val);                     //!< Set the discrete observation
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
        void                            setVirtualPopulationSize(unsigned int populationSize);             //!< Set the virtual population size for the state space
        
        
    private:
        unsigned int                    computeState(const std::string &symbol) const;      //!< Compute the internal state value for this character.
        
        unsigned int                    state;
//      size_t                          stateIndex;
        
        unsigned int                    virtualPopulationSize;                              //!< The virtual population size of the Pomo model (by default, 10)
    };
    
}

#endif

