/**
 * @file
 * This file contains the declaration of PomoState, which is
 * the class for the DNA data types plus two-state polymorphic states in RevBayes.
 * Instead of the usual coding by bitfields, we choose coding by ints.
 *
 * @brief Declaration of PomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: PomoState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef PomoState_H
#define PomoState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {
    
    class PomoState : public DiscreteCharacterState {
        
    public:
        PomoState(void);                                                                    //!< Default constructor
        PomoState(unsigned int vps);                                                        //!< Constructor with virtual population size
        PomoState(const std::string &s);                                                    //!< Constructor with an observation
        
        PomoState*                       clone(void) const;                                  //!< Get a copy of this object
        
        // Discrete character observation functions
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setVirtualPopulationSize(unsigned int populationSize);             //!< Set the virtual population size for the state space
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        
        
    private:
        
        
        unsigned int                    virtualPopulationSize;                              //!< The virtual population size of the Pomo model (by default, 10)
    };
    
}

#endif

