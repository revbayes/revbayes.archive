/**
 * @file
 * This file contains the declaration of DnaState, which is
 * the class for the DNA data types in RevBayes.
 *
 * @brief Declaration of DnaState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef DnaState_H
#define DnaState_H

#include "NucleotideState.h"
#include <ostream>
#include <vector>



class DnaState : public NucleotideState {

    public:
                                        DnaState(void);                                     //!< Default constructor
                                        DnaState(const DnaState& s);                        //!< Copy constructor
                                        DnaState(const char s);                             //!< Constructor with nucleotide observation
        bool                            operator==(const DnaState& x) const;                //!< Equality
        bool                            operator!=(const DnaState& x) const;                //!< Inequality

        // Basic utility functions you should not have to override
        void                            printValue(std::ostream& o) const;                  //!< Print value (for user)

        // Basic utility functions you have to override
        DnaState*                       clone(void) const;                                  //!< Clone object
        const VectorString&             getClass(void) const;                               //!< Get class vector
        std::string                     richInfo(void) const;                               //!< Complete info about object

        // Discrete character observation functions
        const std::string&              getStateLabels(void) const { return stateLabels; }  //!< Get valid state labels
        const char                      getValue(void) const;                               //!< Get the discrete observation

    protected:
        const static std::string        stateLabels;                                        //!< The labels for the possible states
};

#endif

