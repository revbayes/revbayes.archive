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
#include "RlVector.h"
#include <ostream>
#include <vector>


class DiscreteCharacterState : public CharacterState {
    
public:
    virtual bool                            operator==(const CharacterState& x) const = 0;      //!< Equality
    virtual bool                            operator!=(const CharacterState& x) const = 0;      //!< Inequality
    virtual void                            operator++() = 0;                                   //!< Increment
    virtual void                            operator--() = 0;                                   //!< Decrement
 
    // Discrete character observation functions
    virtual void                            addState(size_t pos) = 0;                           //!< Add a character state to the set of character states
    virtual short                           getNumberObservedStates(void) const;                //!< How many states are observed for the character
    virtual const std::string&              getStateLabels(void) const = 0;                     //!< Get valid state labels
    virtual std::string                     getStringValue(void) const = 0;                     //!< Get a representation of the character as a string
    virtual size_t                          getNumberOfStates(void) const = 0;                  //!< Get the number of discrete states for the character
//    virtual char                            getState(void) const = 0;                           //!< Get the discrete observation
    virtual bool                            isGapState(void) const = 0;                         //!< Get whether this is a gapped character state
    virtual bool                            isMissingOrAmbiguous(void) const = 0;               //!< Is the character missing or ambiguous
    virtual void                            setState(size_t pos, bool val) = 0;                 //!< Set the discrete observation
    virtual void                            setIsGapState(bool tf) = 0;                         //!< Set whether this is a gapped character
    
protected:
    DiscreteCharacterState();                                                                   //!< Constructor
};

#endif

