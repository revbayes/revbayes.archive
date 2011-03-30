/**
 * @file
 * This file contains the declaration of Vector, a container type
 * that acts as a base class for all constant vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 *
 * $Id$
 */

#ifndef Vector_H
#define Vector_H

#include "ValueContainer.h"

#include <iostream>
#include <vector>


class Vector : public ValueContainer {

    public:
		virtual                        ~Vector(void) {}                                                     //!< Virtual destructor 

        // Basic utility functions you have to override
        virtual Vector*                 clone(void) const = 0;                                              //!< Clone object
        virtual const VectorString&     getClass(void) const;                                               //!< Get class

        // Vector functions you do not have to override
        void                            pop_back(void);                                                     //!< Drop element at back
        void                            pop_front(void);                                                    //!< Drop element from front
        void                            resize(size_t n);                                                   //!< Resize to new vector of length n

    protected:
                                        Vector(const std::string& elemType);                                //!< Set type spec of container from type of elements
};

#endif

