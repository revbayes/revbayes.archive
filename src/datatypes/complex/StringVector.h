/**
 * @file
 * This file contains the declaration of StringVector, a complex type
 * used to hold string vectors.
 *
 * @brief Declaration of StringVector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 * @extends RbComplex
 *
 * $Id$
 */

#ifndef StringVector_H
#define StringVector_H

#include "RbComplex.h"

#include <iostream>
#include <string>
#include <vector>

class StringVector : public RbComplex {

    public:
            // Constructors and destructor
	        StringVector(void) : RbComplex() {}                 //!< Default constructor (empty vector)
	        StringVector(int n);                                //!< Construct vector of length n of empty strings
            StringVector(const std::string& x);                 //!< Construct vector with one string x
	        StringVector(int n, const std::string& x);          //!< Construct vector with n strings x
	        StringVector(const std::vector<std::string>& x);    //!< Constructor from string vector

        // Basic utility functions
        StringVector*           clone(void) const;                              //!< Clone object
        bool                    equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&     getClass(void) const;                           //!< Get class
        void                    printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string             toString(void) const;                           //!< Complete info about object

        // Overloaded operators and built-in functions
	    std::string&            operator[](size_t i) { return value[i]; }       //!< Index op allowing change
	    const std::string&      operator[](size_t i) const { return value[i]; } //!< Const index op
        bool                    operator==(const StringVector& sv) const;       //!< Equality
        StringVector            operator+(const StringVector& sv) const;        //!< Concatenate
	    void                    push_back(std::string x) { value.push_back(x); }//!< Add element
	    size_t                  size() const { return value.size(); }           //!< Get size

        // Element access functions for parser
        int                     getDim(void) const { return 1; }                //!< Get subscript dimension
        const std::string&      getElementType(void) const;                     //!< Get element type
        const RbObject*         getElement(const IntVector& index) const;       //!< Get element (a copy)
        const IntVector&        getLength(void) const;                          //!< Get length in each dimension
        void                    resize(const IntVector& len);                   //!< Resize
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set element
        void                    setLength(const IntVector& len);                //!< Set length in each dimension

    private:
	    std::vector<std::string>    value;                                      //!< Vector of values
};

#endif
