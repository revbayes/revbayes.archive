/**
 * @file
 * This file contains the declaration of IntVector, a complex type
 * used to hold int vectors.
 *
 * @brief Declaration of IntVector
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

#ifndef IntVector_H
#define IntVector_H

#include "RbComplex.h"

#include <iostream>
#include <vector>


class IntVector : public RbComplex {

    public:
            // Constructors and destructor
            IntVector() : RbComplex() {}                //!< Default constructor (empty vector)
            IntVector(int x);                           //!< Construct vector with one int x
	        IntVector(int n, int x);                    //!< Construct vector with n ints x
	        IntVector(std::vector<int>& x);             //!< Constructor from int vector

        // Static string vector describing class hierarchy
        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*           clone() const;                                  //!< Clone object
        bool                equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }            //!< Get class
        void                printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string         toString(void) const;                           //!< Complete info about object

        // Overloaded operators and built-in functions
	    int&                operator[](int i) { return value[i]; }          //!< Index op allowing change
	    const int&          operator[](int i) const { return value[i]; }    //!< Const index op
	    void                push_back(int x) { value.push_back(x); }        //!< Add element
	    size_t              size() const { return value.size(); }           //!< Get size

        // Element access functions for parser
	    RbObject*               getElement(const IntVector& index) const;       //!< Get element (a copy)
        int                     getElementDim() const { return 1; }             //!< Get dimensions
        const std::string&      getElementType() const;                         //!< Get element type
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set element   

    private:
	    std::vector<int>    value;      //!< Vector of values
};

#endif

