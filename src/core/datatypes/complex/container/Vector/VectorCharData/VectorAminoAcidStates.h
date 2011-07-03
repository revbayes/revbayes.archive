/**
 * @file
 * This file contains the declaration of VectorAminoAcidStates, a complex type
 * used to hold amino acid observations.
 *
 * @brief Declaration of VectorAminoAcidStates
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

#ifndef VectorAminoAcidStates_H
#define VectorAminoAcidStates_H

#include "VectorCharacters.h"
#include <iostream>
#include <vector>

class AminoAcidState;



class VectorAminoAcidStates : public VectorCharacters {

    public:
	                                 VectorAminoAcidStates(void);                                          //!< Default constructor (empty vector)
                                     VectorAminoAcidStates(const VectorAminoAcidStates& x);                //!< Construct vector with a vector of amino acid states

        // Overloaded operators
	    AminoAcidState&              operator[](size_t i);                                                 //!< Index op allowing change
	    const AminoAcidState&        operator[](size_t i) const;                                           //!< Const index op
        bool                         operator==(const VectorAminoAcidStates& x) const;                     //!< Equality
        bool                         operator!=(const VectorAminoAcidStates& x) const;                     //!< Inequality
        VectorAminoAcidStates        operator+(const VectorAminoAcidStates& x) const;                      //!< Concatenate
        VectorAminoAcidStates        operator+(const AminoAcidState& x) const;                             //!< Concatenate

        // Basic utility functions
        VectorAminoAcidStates*       clone(void) const;                                                    //!< Clone object
        const VectorString&          getClass(void) const;                                                 //!< Get class
        void                         printValue(std::ostream& o) const;                                    //!< Print value for user
        std::string                  richInfo(void) const;                                                 //!< Complete info about object
        
        // Vector functions
	    void                         push_back(AminoAcidState x);                                          //!< Add element
        std::vector<AminoAcidState*> getStdVector(void) const;                                             //!< Get the value vector as a std::vector

    protected:
        RbObject*                    getDefaultElement(void) const { return new VectorAminoAcidStates(); } //!< Get default element for empty slots
};

#endif

