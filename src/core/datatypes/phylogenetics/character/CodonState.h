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
        CodonState(size_t n=64);                                                            //!< Default constructor
        CodonState(const std::string &symbol);                                              //!< Constructor with codon observation
//        CodonState(size_t index);                                                           //!< Constructor with codon index
        
        CodonState*                     clone(void) const;                                  //!< Get a copy of this object
        
        // Discrete character observation functions
        AminoAcidState                  getAminoAcidState() const;
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        std::vector<unsigned int>       getTripletStates() const;
        bool                            isStopCodon(void) const;                            //!< Is the codon a stop codon
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        
        
    };
    
}

#endif

