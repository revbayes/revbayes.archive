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
                                        RnaState(size_t n=4);                               //!< Default constructor
                                        RnaState(const std::string &s);                     //!< Constructor with nucleotide observation
        
        RnaState*                       clone(void) const;                                  //!< Get a copy of this object
    
        // Discrete character observation functions
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        RbBitSet&                       getState(void);                                     //!< Get the state (as the bitset)
        const RbBitSet&                 getState(void) const;                               //!< Get the state (as the bitset)
        std::string                     getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        
        
    private:
        
        RbBitSetByte                    state;
        
    };
    
}

#endif

