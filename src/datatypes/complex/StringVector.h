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
	        StringVector(int n);                            //!< Construct empty vector of length n
            StringVector(std::string x);                    //!< Construct vector with one string x
	        StringVector(int n, std::string x);             //!< Construct vector with n strings x
	        StringVector(const StringVector& x);            //!< Copy constructor
	        StringVector(std::vector<std::string>& x);      //!< Constructor from string vector
	        virtual ~StringVector(void) {}                  //!< Destructor

        static const StringVector   rbClass;                //!< Static class attribute

        // Basic utility functions
        RbObject*               clone() const;                                  //!< Clone object
        bool                    equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&     getClass() const { return rbClass; }            //!< Get class
        void                    printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string             toString(void) const;                           //!< General info on object

        // Overloaded operators
        RbObject&               operator=(const RbObject& o);           // Does this work?
        StringVector&           operator=(const StringVector& o);               //!< Assignment
        StringVector            operator+(const StringVector& sv) const;        //!< Concatenate
	    std::string&            operator[](int i) { return value[i]; }          //!< Indexing
	    const std::string&      operator[](int i) const { return value[i]; }    //!< Const indexing

        // Regular functions
	    std::string             get(int i) const { return value[i]; }           //!< Get element
	    void                    push_back(std::string x) { value.push_back(x); }//!< Add element
	    int                     size(void) { return value.size(); }             //!< Get size

    private:
	    std::vector<std::string> value;     //!< String of values
};

#endif
