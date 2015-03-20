/**
 * @file
 * This file contains the declaration of RnaState, which is
 * the class for the Rna data types in RevBayes.
 *
 * @brief Declaration of RnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RnaState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef RnaState_H
#define RnaState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    class RnaState : public DiscreteCharacterState {
    
    public:
                                        RnaState(void);                                     //!< Default constructor
                                        RnaState(const RnaState& s);                        //!< Copy constructor
                                        RnaState(char s);                                   //!< Constructor with nucleotide observation
    
        bool                            operator==(const CharacterState& x) const;          //!< Equality
        bool                            operator!=(const CharacterState& x) const;          //!< Inequality
        bool                            operator<(const CharacterState& d) const;           //!< Less than
        void                            operator++();                                       //!< Increment
        void                            operator++(int i);                                  //!< Increment
        void                            operator+=(int i);                                  //!< Increment
        void                            operator--();                                       //!< Decrement
        void                            operator--(int i);                                  //!< Decrement
        void                            operator-=(int i);                                  //!< Decrement
    
        RnaState*                       clone(void) const;                                  //!< Get a copy of this object
    
        // Discrete character observation functions
        void                            addState(char symbol);                              //!< Add a character state to the set of character states
        std::string                     getDatatype(void) const;                            //!< Get the datatype as a common string.
        unsigned int                    getNumberObservedStates(void) const;                //!< How many states are observed for the character
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        size_t                          getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
        unsigned long                   getState(void) const;                               //!< Get the discrete observation
        size_t                          getStateIndex(void) const;
        bool                            isAmbiguous(void) const;                            //!< Is the character missing or ambiguous
        bool                            isGapState(void) const;                             //!< Get whether this is a gapped character state
        void                            setGapState(bool tf);                               //!< Set whether this is a gapped character
        void                            setState(char symbol);                              //!< Set the discrete observation
        void                            setState(size_t pos, bool val);                     //!< Set the discrete observation
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
    
    private:
        char                            computeState(char symbol) const;                    //!< Compute the internal state value for this character.
    
        char                            state;
        size_t                          stateIndex;
    
    };
    
}

#endif

