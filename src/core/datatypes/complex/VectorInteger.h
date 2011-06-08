/**
 * @file
 * This file contains the declaration of VectorInteger, a complex type
 * used to hold int vectors.
 *
 * @brief Declaration of VectorInteger
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

#ifndef VectorInteger_H
#define VectorInteger_H

#include "Integer.h"
#include "Vector.h"

#include <iostream>
#include <vector>


class VectorInteger : public Vector {

    public:
        // Constructors and destructor
                                    VectorInteger(void);                                            //!< Default constructor (empty vector)
                                    VectorInteger(int x);                                           //!< Construct vector with one int x
                                    VectorInteger(size_t n, int x);                                 //!< Construct vector with n ints x
                                    VectorInteger(const std::vector<int>& x);                       //!< Constructor from int vector
                                    VectorInteger(const std::vector<size_t>& x);                    //!< Constructor from size_t vector
                                    VectorInteger(const VectorNatural& x);                          //!< Constructor from vector of natural numbers
                                    VectorInteger(const ContainerIterator& x);                      //!< Constructor from container iterator

        // Overloaded operators
        int&                        operator[](size_t i);                                           //!< Index op allowing change
        const int&                  operator[](size_t i) const;                                     //!< Const index op
        bool                        operator==(const VectorInteger& x) const;                       //!< Equals comparison
        bool                        operator!=(const VectorInteger& x) const;                       //!< Not equals comparison
        
        // Basic utility functions
        VectorInteger*              clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        std::string                 toIndexString(void) const;                                      //!< Convert to an element or subcontainer index string ([1][2], [][3] etc)

        // Vector functions, including STL-like functions
        std::vector<int>            getValue(void) const;                                           //!< Get value as STL int vector
        void                        push_back(int x);                                               //!< Append element to end
        void                        push_front(int x);                                              //!< Add element in front
        void                        setValue(const VectorInteger& x);                               //!< Set the value
        void                        setValue(const std::vector<int>& x);                            //!< Set the value

    protected:
        RbObject*                   getDefaultElement(void) const { return new Integer(); }         //!< Get default element for empty slots

};

#endif


