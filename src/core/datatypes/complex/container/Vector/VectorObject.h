/**
 * @file
 * This file contains the declaration of VectorObject, a
 * complex type used to hold generic vectors of objects.
 *
 * @brief Declaration of VectorObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef VectorObject_H
#define VectorObject_H

#include "Vector.h"

#include <iostream>
#include <vector>


/** Class for vector of objects */
class VectorObject : public Vector {
    
    public:
                                        VectorObject(void);                                             //!< Default constructor (empty vector)

        // Overloaded operators
        const RbObject*                 operator[](size_t i) const;                                     //!< Index op (const)
        
        // Basic utility functions
        virtual VectorObject*           clone(void) const;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                           //!< Get class
        virtual std::string             richInfo(void) const;                                           //!< Complete info about object
        
        // Vector functions, including STL-like functions
        virtual void                    push_back(RbObject* x);                                         //!< Append element to end
        
    protected:
        RbObject*                       getDefaultElement(void) const { return NULL; }                  //!< Get default element for empty slots
};

#endif

