/**
 * @file
 * This file contains the declaration of VectorNatural, a
 * complex type used to hold vectors of natural numbers.
 *
 * @brief Declaration of VectorNatural
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id:$
 */

#ifndef VectorNatural_H
#define VectorNatural_H

#include "Natural.h"
#include "Vector.h"

#include <iostream>
#include <vector>


/**
 * Vector of natural numbers
 *
 * This class is not derived from VectorInteger for several reasons,
 * including the fact that doing so would make it impossible to
 * protect the subscript operator return.
 */
class VectorNatural : public Vector {

    public:
                                    VectorNatural(void);                                            //!< Default constructor (empty vector)
                                    VectorNatural(const int x);                                     //!< Construct vector with one number x
                                    VectorNatural(const unsigned int x);                            //!< Construct vector with one number x
                                    VectorNatural(size_t n, int x);                                 //!< Construct vector with n numbers x
                                    VectorNatural(size_t n, unsigned int x);                        //!< Construct vector with n numbers x
                                    VectorNatural(const std::vector<int>& x);                       //!< Constructor from int vector
                                    VectorNatural(const std::vector<unsigned int>& x);              //!< Constructor from unsigned int vector
                                    VectorNatural(const VectorInteger& x);                          //!< Constructor from vector of Integer
                                    VectorNatural(const ContainerIterator& x);                      //!< Constructor from container iterator

        // Overloaded operators
        unsigned int                operator[](size_t i) const;                                     //!< Index op (value, no reference - no modification)
        bool                        operator==(const VectorNatural& x) const;                       //!< Equals comparison
        bool                        operator!=(const VectorNatural& x) const;                       //!< Not equals comparison
        
        // Basic utility functions
        VectorNatural*              clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        std::string                 richInfo(void) const;                                           //!< Complete info about object
        std::string                 toIndexString(void) const;                                      //!< Convert to an element index string ([1], [3][1] etc)

        // Vector functions, including STL-like functions
        std::vector<int>            getValue(void) const;                                           //!< Get value as STL int vector
        std::vector<unsigned int>   getUnsignedValue(void) const;                                   //!< Get value as STL unsigned int vector
        void                        push_back(int x);                                               //!< Append element to end
        void                        push_front(int x);                                              //!< Add element in front
        void                        setValue(const std::vector<int>& x);                            //!< Set the value using STL vector of int
        void                        setValue(const std::vector<unsigned int>& x);                   //!< Set the value using STL vector of unsigned int
        void                        setValue(const VectorNatural& x);                               //!< Set the value using VectorNatural
        void                        setValue(const VectorInteger& x);                               //!< Set the value using VectorInteger

    protected:
        RbObject*                   getDefaultElement(void) const { return new Natural(); }         //!< Get default element for empty slots

};

#endif

