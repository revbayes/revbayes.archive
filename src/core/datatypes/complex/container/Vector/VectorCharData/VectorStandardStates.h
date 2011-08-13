/**
 * @file
 * This file contains the declaration of VectorStandardStates, a complex type
 * used to hold Standard observations.
 *
 * @brief Declaration of VectorStandardStates
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

#ifndef VectorStandardStates_H
#define VectorStandardStates_H

#include "VectorCharacters.h"
#include <iostream>
#include <vector>

class StandardState;



class VectorStandardStates : public VectorCharacters {

    public:
	                                VectorStandardStates(void);                                          //!< Default constructor (empty vector)
                                    VectorStandardStates(const VectorStandardStates& x);                 //!< Construct vector with a vector of Standard states

        // Overloaded operators
	    StandardState&              operator[](size_t i);                                                //!< Index op allowing change
	    const StandardState&        operator[](size_t i) const;                                          //!< Const index op
        bool                        operator==(const VectorStandardStates& x) const;                     //!< Equality
        bool                        operator!=(const VectorStandardStates& x) const;                     //!< Inequality
        VectorStandardStates        operator+(const VectorStandardStates& x) const;                      //!< Concatenate
        VectorStandardStates        operator+(const StandardState& x) const;                             //!< Concatenate

        // Basic utility functions
        VectorStandardStates*       clone(void) const;                                                   //!< Clone object
        const VectorString&         getClass(void) const;                                                //!< Get class
        void                        printValue(std::ostream& o) const;                                   //!< Print value for user
        std::string                 richInfo(void) const;                                                //!< Complete info about object
        
        // Vector functions
	    void                        push_back(StandardState x);                                          //!< Add element
        std::vector<StandardState*> getStdVector(void) const;                                            //!< Get the value vector as a std::vector

};

#endif

