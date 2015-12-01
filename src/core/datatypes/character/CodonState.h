/**
 * @file
 * This file contains the declaration of CodonState, which is
 * the class for the Codon data types in RevBayes.
 *
 * @brief Declaration of CodonState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: CodonState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef CodonState_H
#define CodonState_H

#include "AminoAcidState.h"
#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {
    
    class CodonState : public DiscreteCharacterState {
        
    public:
        CodonState(void);                                                                   //!< Default constructor
        CodonState(const std::string &symbol);                                              //!< Constructor with codon observation
        CodonState(size_t index);                                                           //!< Constructor with codon index
        
        bool                            operator==(const CharacterState& x) const;          //!< Equality
        bool                            operator!=(const CharacterState& x) const;          //!< Inequality
        bool                            operator<(const CharacterState& x) const;           //!< Less than
        void                            operator++();                                       //!< Increment
        void                            operator++(int i);                                  //!< Increment
        void                            operator+=(int i);                                  //!< Increment
        void                            operator--();                                       //!< Decrement
        void                            operator--(int i);                                  //!< Decrement
        void                            operator-=(int i);                                  //!< Decrement
        
        CodonState*                     clone(void) const;                                  //!< Get a copy of this object
        
        // Discrete character observation functions
        void                            addState(const std::string &symbol);                //!< Add a character state to the set of character states
        AminoAcidState                  getAminoAcidState() const;
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        unsigned int                    getNumberObservedStates(void) const;                //!< How many states are observed for the character
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        size_t                          getNumberOfStates(void) const;                      //!< Get the number of discrete states for the character
        unsigned long                   getState(void) const;                               //!< Get the discrete observation
        size_t                          getStateIndex(void) const;
        std::vector<unsigned int>       getTripletStates() const;
        bool                            isAmbiguous(void) const;                            //!< Is the character missing or ambiguous
        bool                            isStopCodon(void) const;                            //!< Is the codon a stop codon
        void                            setStateByIndex(size_t index);                      //!< Set the discrete observation
        void                            setState(const std::string &symbol);                //!< Set the discrete observation
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
        
    private:
        unsigned int                    computeState(const std::string &symbol) const;      //!< Compute the internal state value for this character.
        
        unsigned int                    state;
        size_t                          stateIndex;
        
    };
    
}

#endif

