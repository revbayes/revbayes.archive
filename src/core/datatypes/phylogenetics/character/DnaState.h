/**
 * @file
 * This file contains the declaration of DnaState, which is
 * the class for the DNA data types in RevBayes.
 *
 * @brief Declaration of DnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-05-24 09:58:04 +0200 (Thu, 24 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DnaState.h 1568 2012-05-24 07:58:04Z hoehna $
 */

#ifndef DnaState_H
#define DnaState_H

#include "DiscreteCharacterState.h"
#include <ostream>
#include <set>

namespace RevBayesCore {

    class DnaState : public DiscreteCharacterState {
    
    public:
                                        DnaState(void);                                     //!< Default constructor
                                        DnaState(const std::string &s);                     //!< Constructor with nucleotide observation

        DnaState*                       clone(void) const;                                  //!< Get a copy of this object

        // Discrete character observation functions
        std::string                     getDataType(void) const;                            //!< Get the datatype as a common string.
        const std::string&              getStateLabels(void) const;                         //!< Get valid state labels
        std::string                     getStringValue(void) const;                         //!< Get a representation of the character as a string
        void                            setState(const std::string &symbol);                //!< Compute the internal state value for this character.
        
    
    };
    
}

#endif

