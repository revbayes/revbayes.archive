/**
 * @file
 * This file contains the declaration of RnaState, which is
 * the class for the RNA data types in RevBayes.
 *
 * @brief Declaration of RnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef RnaState_H
#define RnaState_H

#include "NucleotideState.h"
#include <ostream>
#include <vector>



class RnaState : public NucleotideState {

    public:
        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        RnaState*                       clone(void) const;                                  //!< Clone object
        const VectorString&             getClass(void) const;                               //!< Get class vector
        std::string                     richInfo(void) const;                               //!< Complete info about object

        // Discrete character observation functions
        const std::string&              getStateLabels(void) const { return stateLabels; }  //!< Get valid state labels
        const char                      getValue(void) const;                               //!< Get the discrete observation

    protected:
                                        RnaState(void);                                     //!< Default constructor
                                        RnaState(const RnaState& s);                        //!< Copy constructor
                                        RnaState(const char s);                             //!< Constructor with nucleotide observation
        const static std::string        stateLabels;                                        //!< The labels for the possible states
};

#endif

