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
#include "RbBitSetGeneral.h"

#include <ostream>
#include <set>

namespace RevBayesCore {

    class StandardState : public DiscreteCharacterState {
    
    public:
                                        StandardState(size_t n=4);                                  //!< Default constructor
                                        StandardState(const std::string &s);                        //!< Constructor with nucleotide observation
                                        StandardState(const std::string& s, const std::string& l);
        
        StandardState*                  clone(void) const;                                          //!< Get a copy of this object
    
        // Discrete character observation functions
        std::string                     getDataType(void) const;                                    //!< Get the datatype as a common string.
        RbBitSet&                       getState(void);                                             //!< Get the state (as the bitset)
        const RbBitSet&                 getState(void) const;                                       //!< Get the state (as the bitset)
        std::string                     getStateLabels(void) const;                                 //!< Get valid state labels
    
    private:
                
        std::string                     labels;
        RbBitSetGeneral                 state;
        
    };

}

#endif

