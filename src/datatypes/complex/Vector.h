/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * that acts as a base class for all vectors.
 *
 * @brief Declaration of Vector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef Vector_H
#define Vector_H

#include "RbComplex.h"

#include <iostream>
#include <vector>

class Vector : public RbComplex {

    public:
        // destructor
		virtual                     ~Vector(void) {}                                              //!< Virtual destructor 

        // Basic utility functions, most of which are carried through as pure virtual from RbComplex
        virtual Vector*              clone(void) const = 0;                                       //!< Clone object
        virtual bool                 equals(const RbObject* obj) const = 0;                       //!< Equals comparison
		virtual const VectorString&  getClass(void) const;                                        //!< Get class vector
		virtual void                 printValue(std::ostream& o) const = 0;                       //!< Print value for user
		virtual std::string          toString(void) const = 0;                                    //!< Complete info 

        // Overloaded operators and built-in functions
        int&                        operator[](size_t i) { return value[i]; }                     //!< Index op allowing change
        const int&                  operator[](size_t i) const { return value[i]; }               //!< Const index op
        bool                        operator==(const VectorInteger& x) const;                     //!< Equals comparison
        bool                        operator!=(const VectorInteger& x) const;                     //!< Not equals comparison
        void                        clear(void) { value.clear(); }                                //!< Clear
        void                        pop_back(void) { value.pop_back(); }                          //!< Drop element
        void                        push_back(int x) { value.push_back(x); }                      //!< Append element to end
        void                        push_front(int x) { value.insert(value.begin(), x); }         //!< Add element in front
        void                        resize(size_t n) { value.resize(n); }                         //!< Resize
        size_t                      size(void) const { return value.size(); }                     //!< Get size

        // Element access functions for parser
        int                         getDim(void) const { return 1; }                              //!< Get subscript dimensions
        const std::string&          getElementType(void) const;                                   //!< Get element type
        const RbObject*             getElement(const VectorInteger& index) const;                 //!< Get element (read-only)
        const VectorInteger&        getLength(void) const;                                        //!< Get length in each dim
        void                        resize(const VectorInteger& len);                             //!< Resize
        void                        setElement(const VectorInteger& index, RbObject* val);        //!< Set element
        void                        setLength(const VectorInteger& len);                          //!< Set length in each dim

    private:
        bool                        isRowVector;                                                  //!< Indicates whether the vector is a row vector (true) or a column vector (false)
};

#endif


