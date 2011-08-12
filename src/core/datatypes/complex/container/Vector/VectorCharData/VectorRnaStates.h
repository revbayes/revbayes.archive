/**
 * @file
 * This file contains the declaration of VectorDnaStates, a complex type
 * used to hold DNA nucleotide observations.
 *
 * @brief Declaration of VectorDnaStates
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends VectorCharacters
 *
 * $Id$
 */

#ifndef VectorRnaStates_H
#define VectorRnaStates_H

#include "VectorCharacters.h"
#include <iostream>
#include <vector>

class RnaState;



class VectorRnaStates : public VectorCharacters {

    public:
	                                VectorRnaStates(void);                                          //!< Default constructor (empty vector)
                                    VectorRnaStates(const VectorRnaStates& x);                      //!< Construct vector with a vector of RNA states

        // Overloaded operators
	    RnaState&                   operator[](size_t i);                                           //!< Index op allowing change
	    const RnaState&             operator[](size_t i) const;                                     //!< Const index op
        bool                        operator==(const VectorRnaStates& x) const;                     //!< Equality
        bool                        operator!=(const VectorRnaStates& x) const;                     //!< Inequality
        VectorRnaStates             operator+(const VectorRnaStates& x) const;                      //!< Concatenate
        VectorRnaStates             operator+(const RnaState& x) const;                             //!< Concatenate

        // Basic utility functions
        VectorRnaStates*            clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        void                        printValue(std::ostream& o) const;                              //!< Print value for user
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        
        // Vector functions
	    void                        push_back(RnaState x);                                          //!< Add element
        std::vector<RnaState*>      getStdVector(void) const;                                       //!< Get the value vector as a std::vector

    protected:
//        RbObject*                   getDefaultElement(void) const { return new VectorRnaStates(); } //!< Get default element for empty slots
};

#endif

