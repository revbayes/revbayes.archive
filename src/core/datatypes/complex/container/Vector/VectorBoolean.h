/**
 * @file
 * This file contains the declaration of VectorBoolean, a complex type
 * used to hold boolean vectors.
 *
 * @brief Declaration of VectorBoolean
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Vector
 *
 * $Id$
 */

#ifndef VectorBoolean_H
#define VectorBoolean_H

#include "Vector.h"

#include <iostream>
#include <vector>


class VectorBoolean : public Vector {

    public:
        // Constructors and destructor
                                    VectorBoolean(void);                                            //!< Default constructor (empty vector)
                                    VectorBoolean(bool x);                                          //!< Construct vector with one bool x
                                    VectorBoolean(size_t n, bool x);                                //!< Construct vector with n bools x
                                    VectorBoolean(const std::vector<bool>& x);                      //!< Constructor from bool vector
                                    VectorBoolean(const std::vector<int>& x);                       //!< Constructor from int vector

        // Overloaded operators
        bool&                       operator[](size_t i);                                           //!< Index op allowing change
        const bool&                 operator[](size_t i) const;                                     //!< Const index op
        bool                        operator==(const VectorBoolean& x) const;                       //!< Equals comparison
        bool                        operator!=(const VectorBoolean& x) const;                       //!< Not equals comparison
        
        // Basic utility functions
        VectorBoolean*              clone(void) const;                                              //!< Clone object
        const VectorString&         getClass(void) const;                                           //!< Get class
        std::string                 richInfo(void) const;                                           //!< Complete info about object

        // Vector functions, including STL-like functions
        std::vector<bool>           getValue(void) const;                                           //!< Get value
        void                        push_back(bool x);                                              //!< Append element to end
        void                        push_front(bool x);                                             //!< Add element in front
        void                        setValue(const VectorBoolean& x);                               //!< Set the value
        void                        setValue(const std::vector<bool>& x);                           //!< Set the value

    protected:
//        RbObject*                   getDefaultElement(void) const { return new Boolean(); }         //!< Get default element for empty slots

};

#endif

