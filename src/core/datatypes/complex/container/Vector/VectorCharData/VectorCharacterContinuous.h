/**
 * @file
 * This file contains the declaration of VectorCharacterContinuous, a complex type
 * used to hold continuous character observations.
 *
 * @brief Declaration of VectorCharacterContinuous
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

#ifndef VectorCharacterContinuous_H
#define VectorCharacterContinuous_H

#include "VectorCharacters.h"
#include <iostream>
#include <vector>

class CharacterContinuous;



class VectorCharacterContinuous : public VectorCharacters {

    public:
	                                                 VectorCharacterContinuous(void);                                          //!< Default constructor (empty vector)
                                                     VectorCharacterContinuous(const VectorCharacterContinuous& x);            //!< Construct vector with a vector of continuous characters

        // Overloaded operators
	    CharacterContinuous&                         operator[](size_t i);                                                     //!< Index op allowing change
	    const CharacterContinuous&                   operator[](size_t i) const;                                               //!< Const index op
        bool                                         operator==(const VectorCharacterContinuous& x) const;                     //!< Equality
        bool                                         operator!=(const VectorCharacterContinuous& x) const;                     //!< Inequality
        VectorCharacterContinuous                    operator+(const VectorCharacterContinuous& x) const;                      //!< Concatenate
        VectorCharacterContinuous                    operator+(const CharacterContinuous& x) const;                            //!< Concatenate

        // Basic utility functions
        VectorCharacterContinuous*                   clone(void) const;                                                        //!< Clone object
        const VectorString&                          getClass(void) const;                                                     //!< Get class
        void                                         printValue(std::ostream& o) const;                                        //!< Print value for user
        std::string                                  richInfo(void) const;                                                     //!< Complete info about object
        
        // Vector functions
	    void                                         push_back(CharacterContinuous x);                                         //!< Add element
        std::vector<CharacterContinuous*>            getStdVector(void) const;                                                 //!< Get the value vector as a std::vector

    protected:
        RbObject*                                    getDefaultElement(void) const { return new VectorCharacterContinuous(); } //!< Get default element for empty slots
};

#endif

