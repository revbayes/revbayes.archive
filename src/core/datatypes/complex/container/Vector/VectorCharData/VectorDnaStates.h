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
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef VectorDnaStates_H
#define VectorDnaStates_H

#include "VectorCharacters.h"
#include <iostream>
#include <vector>

class DnaState;



class VectorDnaStates : public VectorCharacters {

    public:
	                                VectorDnaStates(void);                                          //!< Default constructor (empty vector)
                                    VectorDnaStates(const VectorDnaStates& x);                      //!< Construct vector with a vector of DNA states

        // Overloaded operators
	    DnaState&                   operator[](size_t i);                                           //!< Index op allowing change
	    const DnaState&             operator[](size_t i) const;                                     //!< Const index op
        bool                        operator==(const VectorDnaStates& x) const;                     //!< Equality
        bool                        operator!=(const VectorDnaStates& x) const;                     //!< Inequality
        VectorDnaStates             operator+(const VectorDnaStates& x) const;                      //!< Concatenate
        VectorDnaStates             operator+(const DnaState& x) const;                             //!< Concatenate

        // Basic utility functions
        VectorDnaStates*            clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        void                        printValue(std::ostream& o) const;                              //!< Print value for user
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        
        // Vector functions
	    void                        push_back(DnaState x);                                          //!< Add element
        std::vector<DnaState*>      getStdVector(void) const;                                       //!< Get the value vector as a std::vector

    protected:
        RbObject*                   getDefaultElement(void) const { return new VectorDnaStates(); } //!< Get default element for empty slots
};

#endif

