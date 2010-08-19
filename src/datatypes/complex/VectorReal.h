/**
 * @file
 * This file contains the declaration of Vector, a complex type
 * used to hold double vectors.
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

#include "IntVector.h"
#include "Container.h"

#include <iostream>
#include <string>
#include <vector>

class Vector : public Container {

    public:
                                    Vector(void) : Container() {}                              //!< Default constructor (empty vector)
                                    Vector(double x);                                          //!< Construct vector with one double x
	                                Vector(int n, double x);                                   //!< Construct vector with n doubles x
	                                Vector(std::vector<double>& x);                            //!< Constructor from double vector

        // Basic utility functions
        virtual Vector*             clone(void) const;                                         //!< Clone object
        virtual bool                equals(const RbObject* obj) const;                         //!< Equals comparison
        virtual const VectorString& getClass(void) const;                                      //!< Get class vector
        virtual void                printValue(std::ostream& o) const;                         //!< Print value for user
        virtual std::string         toString(void) const;                                      //!< Complete info about object

        // Overloaded operators and built-in functions
        virtual double&             operator[](size_t i) { return value[i]; }                  //!< Index op allowing change
        const double&               operator[](size_t i) const { return value[i]; }            //!< Const index op
        void                        clear(void) { value.clear(); }                             //!< Clear
        virtual void                pop_back(void) { value.pop_back(); }                       //!< Drop element
        virtual void                push_back(double x) { value.push_back(x); }                //!< Append element to end
        virtual void                push_front(double x) { value.insert(value.begin(), x); }   //!< Add element in front
        void                        resize(size_t n) { resize(IntVector(int(n))); }            //!< Resize
        size_t                      size(void) const { return value.size(); }                  //!< Get size

        // Getter
        std::vector<double>&        getValue(void) { return value; }                           //!< Get value directly

        // Element access functions for parser
	    int                         getDim(void) const { return 1; }                           //!< Get subscript dimensions
        const std::string&          getElementType(void) const;                                //!< Get element type
        const RbObject*             getElement(const IntVector& index) const;                  //!< Get element (read-only)
        const IntVector&            getLength(void) const;                                     //!< Get length in each dim
        virtual void                resize(const IntVector& len);                              //!< Resize
        virtual void                setElement(const IntVector& index, RbObject* val);         //!< Set element
        virtual void                setLength(const IntVector& len);                           //!< Set length in each dim

	protected:
        RbObject*                   convertTo(const std::string& type) const;                  //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;            //!< Is convertible to type and dim?
	    std::vector<double>         value;                                                     //!< Vector of values
};

#endif


