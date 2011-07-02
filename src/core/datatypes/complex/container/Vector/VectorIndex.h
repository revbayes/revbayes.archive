/**
 * @file
 * This file contains the declaration of VectorIndex, a
 * complex type used to hold vectors of indices (ints or
 * strings) in the parser.
 *
 * @brief Declaration of VectorIndex
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef VectorIndex_H
#define VectorIndex_H

#include "VectorObject.h"

#include <iostream>
#include <vector>


/** Class for vector of indices (strings or ints) */
class VectorIndex : public VectorObject {
    
    public:
                                    VectorIndex(void);                                             //!< Default constructor (empty vector)
    
        // Overloaded operators
        const RbObject*             operator[](size_t i) const;                                     //!< Index op (const)
        
        // Basic utility functions
        VectorIndex*                clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        std::string                 toIndexString(void) const;                                      //!< Convert to an element index string ([1], [3][1], ["b"] etc)
        
        // Vector functions, including STL-like functions
        int                         getInt( size_t i ) const;                                       //!< Get int value of index i, if any
        std::string                 getString( size_t i ) const;                                    //!< Get string value of index i, if any
        bool                        isEmpty( size_t i ) const;                                      //!< Is element i a negative int, i.e., an empty index?
        void                        push_back(RbObject* x);                                         //!< Append element to end
        void                        push_back(int x);                                               //!< Append int element to end
        void                        push_back(std::string& s);                                      //!< Append string element to end
        
    protected:
        RbObject*                   getDefaultElement(void) const { return NULL; }                  //!< Get default element for empty slots
};

#endif

