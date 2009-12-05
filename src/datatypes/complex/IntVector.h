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
            IntVector(int x);                           //!< Construct vector with one int x
	        IntVector(int n, int x);                    //!< Construct vector with n ints x
	        IntVector(std::vector<int>& x);             //!< Constructor from int vector

        static const StringVector   rbClass;            //!< Static class attribute

        // Basic utility functions
        RbObject*           clone() const;                                  //!< Clone object
        bool                equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector& getClass() const { return rbClass; }            //!< Get class
        void                printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string         toString(void) const;                           //!< General info on object

        // Overloaded operators
        RbObject&           operator=(const RbObject& o) { return (*this); }//!< TODO: remove from all RbObject
	    int&                operator[](int i) { return value[i]; }          //!< Indexing
	    const int&          operator[](int i) const { return value[i]; }    //!< Const indexing

        // Regular functions
	    int                 get(int i) const { return value[i]; }           //!< Get element
	    void                push_back(int x) { value.push_back(x); }        //!< Add element
	    int                 size(void) { return value.size(); }             //!< Get size

    private:
	    std::vector<int>    value;      //!< Vector of values
};

#endif
