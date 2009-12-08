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

#include <iostream>
#include <string>
#include <vector>
#include "RbComplex.h"

class StringVector : public RbComplex {

    public:
            // Constructors and destructor
	        StringVector(void);                             //!< Default constructor (empty vector)
	        StringVector(int n);                            //!< Construct vector of length n of empty strings
            StringVector(std::string x);                    //!< Construct vector with one string x
	        StringVector(int n, std::string x);             //!< Construct vector with n strings x
	        StringVector(const StringVector& x);            //!< Copy constructor
	        StringVector(std::vector<std::string>& x);      //!< Constructor from string vector
	        virtual ~StringVector(void) {}                  //!< Virtual destructor, just in case

        // Basic utility functions
        RbObject*               clone(void) const;                              //!< Clone object
        bool                    equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&     getClass(void) const;                           //!< Get class
        void                    printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string             toString(void) const;                           //!< Complete info about object

        // Overloaded operators and built-in functions
        bool                    operator==(const StringVector& sv) const;       //!< Equality
        StringVector            operator+(const StringVector& sv) const;        //!< Concatenate
	    std::string&            operator[](int i) { return value[i]; }          //!< Index op allowing change
	    const std::string&      operator[](int i) const { return value[i]; }    //!< Const index op
	    void                    push_back(std::string x) { value.push_back(x); }//!< Add element
	    size_t                  size() const { return value.size(); }           //!< Get size

        // Element access functions for parser
	    RbObject*               getElement(const IntVector& index) const;       //!< Get element (a copy)
        int                     getElementDim() const { return 1; }             //!< Get dimensions
        const std::string&      getElementType() const;                         //!< Get element type
        void                    setElement(const IntVector& index, RbObject* val);  //!< Set element   

    private:
	    std::vector<std::string> value;     //!< Vector of values
};

#endif
