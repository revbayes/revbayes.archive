/**
 * @file
 * This file contains the declaration of StandardState, which is
 * the class for the Standard data types in RevBayes.
 *
 * @brief Declaration of StandardState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: StandardState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef StandardState_H
#define StandardState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>



class StandardState : public DiscreteCharacterState {
    
public:
    StandardState(void);                                                                //!< Default constructor
    StandardState(const StandardState& s);                                              //!< Copy constructor
    StandardState(const std::string &s);                                                //!< Constructor with nucleotide observation
    
    bool                            operator==(const CharacterState& x) const;          //!< Equality
    bool                            operator!=(const CharacterState& x) const;          //!< Inequality
    bool                            operator<(const CharacterState& d) const;           //!< Less than
    void                            operator++();                                       //!< Increment
    void                            operator++(int i);                                  //!< Increment
    void                            operator--();                                       //!< Decrement
    void                            operator--(int i);                                  //!< Decrement
    
    StandardState*                  clone(void) const;                                  //!< Get a copy of this object
    static const std::string&       getClassName(void);                                 //!< Get class name
    
    // Discrete character observation functions
    void                            addState(size_t pos);                               //!< Add a character state to the set of character states
    unsigned int                    getNumberObservedStates(void) const;                //!< How many states are observed for the character
    const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
    std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
    size_t                          getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
    unsigned int                    getState(void) const;                               //!< Get the discrete observation
    bool                            isAmbiguous(void) const;                            //!< Is the character missing or ambiguous
    bool                            isGapState(void) const;                             //!< Get whether this is a gapped character state
    void                            setGapState(bool tf);                               //!< Set whether this is a gapped character
    void                            setState(char s);                                   //!< Set the discrete observation
    void                            setState(size_t pos, bool val);                     //!< Set the discrete observation
    void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
    
private:
    std::string                     labels;
    unsigned int                    state;
    
};

#endif

