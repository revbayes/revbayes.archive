/**
 * @file
 * This file contains the declaration of DiscreteCharacterState, which is
 * the abstract base class for character observations, such as DNA, RNA, 
 * etc. data types in RevBayes.
 *
 * @brief Declaration of DiscreteCharacterState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DiscreteCharacterState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef DiscreteCharacterState_H
#define DiscreteCharacterState_H

#include "CharacterState.h"

#include <ostream>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    class DiscreteCharacterState : public CharacterState {
    
    public:
        virtual                                ~DiscreteCharacterState(void) {}
        
        virtual bool                            operator==(const CharacterState& x) const = 0;      //!< Equality
        virtual bool                            operator!=(const CharacterState& x) const = 0;      //!< Inequality
        virtual void                            operator++() = 0;                                   //!< Increment
        virtual void                            operator++(int i) = 0;                              //!< Increment
        virtual void                            operator+=(int i) = 0;                              //!< Increment
        virtual void                            operator--() = 0;                                   //!< Decrement
        virtual void                            operator--(int i) = 0;                              //!< Decrement
        virtual void                            operator-=(int i) = 0;                              //!< Decrement
 
        virtual DiscreteCharacterState*         clone(void) const = 0;                              //!< Get a copy of this object

        // Discrete character observation functions
        virtual void                            addState(const std::string &symbol) = 0;            //!< Add a character state to the set of character states
        virtual unsigned int                    getNumberObservedStates(void) const = 0;            //!< How many states are observed for the character
        virtual size_t                          getNumberOfStates(void) const = 0;                  //!< Get the number of discrete states for the character
        virtual const std::string&              getStateLabels(void) const = 0;                     //!< Get valid state labels
        virtual std::string                     getStringValue(void) const = 0;                     //!< Get a representation of the character as a string
        virtual unsigned long                   getState(void) const = 0;                           //!< Get the discrete observation
        virtual size_t                          getStateIndex(void) const = 0;                      //!< Get the state index
        virtual bool                            isAmbiguous(void) const = 0;                        //!< Is the character missing or ambiguous
        virtual void                            setToFirstState(void) = 0;                          //!< Set this character state to the first (lowest) possible state
        virtual void                            setStateByIndex(size_t index) = 0;                  //!< Set the discrete observation
        virtual void                            setState(const std::string &symbol) = 0;            //!< Set the discrete observation
//      virtual void                            setState(size_t pos, bool val) = 0;                 //!< Set the discrete observation
    
    protected:
                                                DiscreteCharacterState(void) {}                     //!< Constructor

    };
    
}

#endif

