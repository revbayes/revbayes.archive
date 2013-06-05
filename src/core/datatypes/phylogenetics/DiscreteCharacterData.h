/**
 * @file
 * This file contains the declaration of Alignment, which is
 * class that holds a character matrix in RevBayes.
 *
 * @brief Declaration of Alignment
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2013-05-30 12:24:09 +0200 (Thu, 30 May 2013) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: CharacterData.h 2043 2013-05-30 10:24:09Z hoehna $
 */

#ifndef DiscreteCharacterData_H
#define DiscreteCharacterData_H

#include "CharacterData.h"
#include "CharacterState.h"
#include "MatrixReal.h"
#include "TaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace RevBayesCore {
    
    template<class charType>
    class DiscreteCharacterData : public CharacterData<charType> {
        
    public:
        DiscreteCharacterData();                                                            //!< Constructor requires character type
        
        
        // implemented methods of the Cloneable interface
        DiscreteCharacterData<charType>*    clone(void) const;
        
        MatrixReal                          computeStateFrequencies(void) const;
        
    private:
        
    };
    
    
}


#include "CharacterState.h"
#include "CharacterData.h"
#include "RbException.h"
#include "TaxonData.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/* Constructor */
template<class charType>
RevBayesCore::DiscreteCharacterData<charType>::DiscreteCharacterData() : CharacterData<charType>() {
}


template<class charType>
RevBayesCore::MatrixReal RevBayesCore::DiscreteCharacterData<charType>::computeStateFrequencies( void ) const {
    
    charType tmp;
    std::string labels = tmp.getStateLabels();
    size_t alphabetSize = tmp.getNumberOfStates();
    size_t numSequences = this->sequenceNames.size();
    MatrixReal m(numSequences,alphabetSize);
    for (size_t i = 0; i < numSequences; ++i) 
    {
        const TaxonData<charType>& seq = this->getTaxonData(i);
        size_t l = seq.size();
        double nonGapSeqLength = 0.0;
        std::vector<double> stateCounts(alphabetSize);
        for (size_t j = 0; j < l; ++j) 
        {
            const charType& c = seq[j];
            
            if ( !c.isGapState() ) 
            {
                nonGapSeqLength++;
            
                unsigned int state = c.getState();
                double numObservedStates = c.getNumberObservedStates();
            
                size_t index = 0;
                do 
                {
                
                    if ( (state & 1) == 1 ) {
                        // add a uniform probability of having observed each of the ambiguous characters
                        stateCounts[index] += 1.0 / numObservedStates;
                    }
                    state >>= 1;
                    ++index;
                } while ( state != 0 );
            }
        } // finished loop over sequence
        
        // set the observed state frequencies for this sequence into the matrix
        std::vector<double> &observedFreqs = m[i];
        for (size_t j = 0; j < alphabetSize; ++j) {
            observedFreqs[j] = stateCounts[j] / nonGapSeqLength;
        }
    }
    
    return m;
}


/** clone the object */
template<class charType>
RevBayesCore::DiscreteCharacterData<charType>* RevBayesCore::DiscreteCharacterData<charType>::clone( void ) const {
    
    return new DiscreteCharacterData<charType>(*this);
}



#endif

