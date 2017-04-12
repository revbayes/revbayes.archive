#ifndef AminoAcidState_H
#define AminoAcidState_H

#include "DiscreteCharacterState.h"
#include "RbBitSetGeneral.h"

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
                                        AminoAcidState(size_t n=20);                        //!< Default constructor
                                        AminoAcidState(const std::string &s);               //!< Constructor with nucleotide observation
        
        AminoAcidState*                 clone(void) const;                                  //!< Get a copy of this object

        // Discrete character observation functions
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        RbBitSet&                       getState(void);                                     //!< Get the state (as the bitset)
        const RbBitSet&                 getState(void) const;                               //!< Get the state (as the bitset)
        std::string                     getStateLabels(void) const;                         //!< Get valid state labels
        
    private:
        
        RbBitSetGeneral                 state;

    };
    
}

#endif

