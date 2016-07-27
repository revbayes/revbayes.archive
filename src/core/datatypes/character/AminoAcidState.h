#ifndef AminoAcidState_H
#define AminoAcidState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    
    
    /**
     * Amino Acid State class.
     *
     * Amino acid states are represented by their own data structure (this one).
     * Internally we store the amino acid state as an unsigned integer number.
     * We use the first 20 bits of this variable to mark which state is set.
     * This allows us to use ambiguous observations.
     * We provide the common functions, see DiscreteCharacterState.h
     *
     * \copyright (c) Copyright 2009- (GPL version 3)
     * \author The RevBayes Development Core Team (Sebastian Hoehna)
     * \since Version 1.0, 2012-06-21
     */
    class AminoAcidState : public DiscreteCharacterState {
    
    public:
                                        AminoAcidState(void);                               //!< Default constructor
                                        AminoAcidState(const std::string &s);                             //!< Constructor with nucleotide observation
    
        bool                            operator==(const CharacterState& x) const;          //!< Equality
        bool                            operator!=(const CharacterState& x) const;          //!< Inequality
        bool                            operator<(const CharacterState& x) const;           //!< Less than
        void                            operator++();                                       //!< Increment
        void                            operator++(int i);                                  //!< Increment
        void                            operator+=(int i);                                  //!< Increment
        void                            operator--();                                       //!< Decrement
        void                            operator--(int i);                                  //!< Decrement
        void                            operator-=(int i);                                  //!< Decrement
    
        AminoAcidState*                 clone(void) const;                                  //!< Get a copy of this object

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
        void                            setToFirstState(void);                              //!< Set this character state to the first (lowest) possible state
    
    private:
        
        unsigned int                    computeState(const std::string &symbol) const;      //!< Compute the internal state value for this character.
    
        unsigned int                    state;
        size_t                          stateIndex;
    
    };
    
}

#endif

