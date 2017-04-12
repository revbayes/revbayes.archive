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
#include "RbBitSetGeneral.h"

#include <ostream>
#include <set>

namespace RevBayesCore {
    
    class NaturalNumbersState : public DiscreteCharacterState {
        
    public:
        NaturalNumbersState(size_t n=10);                                                      //!< Default constructor
        NaturalNumbersState(const std::string &s, int m);                                   //!< Constructor with an observation
        NaturalNumbersState(int s, int m);                                                  //!< Constructor with an observation
                
        NaturalNumbersState*            clone(void) const;                                  //!< Get a copy of this object
        
        // Discrete character observation functions
        void                            addState(int s);                                    //!< Add the state with the given index.
        void                            addStateDescriptions(const std::vector<std::string>& d);
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        RbBitSet&                       getState(void);                                     //!< Get the state (as the bitset)
        const RbBitSet&                 getState(void) const;                               //!< Get the state (as the bitset)
        std::string                     getStateDescription(void) const;
        std::vector<std::string>        getStateDescriptions(void) const;
        std::string                     getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        
    private:
        
        std::vector<std::string>        state_descriptions;
        RbBitSetGeneral                 state;
    };
//	extern size_t						g_MAX_NAT_NUM_STATES;
}

#endif
