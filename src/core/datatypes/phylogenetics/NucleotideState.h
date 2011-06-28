/**
 * @file
 * This file contains the declaration of NucleotideState, which is
 * the abstract base class for nucleotide observations (DNA & RNA) 
 * data types in RevBayes.
 *
 * @brief Declaration of NucleotideState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef NucleotideState_H
#define NucleotideState_H

#include "CharacterObservationDiscrete.h"
#include <ostream>
#include <vector>



class NucleotideState : public CharacterObservationDiscrete {

    public:
        // Basic utility functions you should not have to override
        virtual void                    printValue(std::ostream& o) const = 0;  //!< Print value (for user)

        // Basic utility functions you have to override
        virtual NucleotideState*        clone(void) const = 0;                  //!< Clone object
        virtual const VectorString&     getClass(void) const;                   //!< Get class vector
        virtual std::string             richInfo(void) const = 0;               //!< Complete info about object

        // Discrete character observation functions
        virtual const std::string&      getStateLabels(void) const = 0;         //!< Get valid state labels
        virtual const char              getValue(void) const = 0;               //!< Get the discrete observation
        void                            setValue(const char s);                 //!< Set the discrete observation

    protected:
                                        NucleotideState(void);                  //!< Default constructor
};

#endif

