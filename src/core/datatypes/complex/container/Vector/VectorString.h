/**
 * @file
 * This file contains the declaration of VectorString, a complex type
 * used to hold string vectors.
 *
 * @brief Declaration of VectorString
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#ifndef VectorString_H
#define VectorString_H

#include "RbString.h"
#include "Vector.h"

#include <iostream>
#include <string>
#include <vector>


const std::string VectorString_name = "String vector";

class VectorString : public Vector {

    public:
	                                VectorString(void);                                             //!< Default constructor (empty vector)
                                    VectorString(const std::string& x);                             //!< Construct vector with one string x
	                                VectorString(const std::vector<std::string>& x);                //!< Constructor from string vector

        // Overloaded operators
	    std::string&                operator[](size_t i);                                           //!< Index op allowing change
	    const std::string&          operator[](size_t i) const;                                     //!< Const index op
        bool                        operator==(const VectorString& x) const;                        //!< Equality
        bool                        operator!=(const VectorString& x) const;                        //!< Inequality
        VectorString                operator+(const RbString& x) const;                             //!< Concatenate
        VectorString                operator+(const VectorString& x) const;                         //!< Concatenate
        VectorString                operator+(const std::string& x) const;                          //!< Concatenate

        // Basic utility functions
        VectorString*               clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        
        // Vector functions
	    void                        push_back(std::string x);                                       //!< Add element
    
        std::vector<std::string>    getStdVector(void) const;                                       //!< Get the value vector as a std::vector
    
};

#endif

