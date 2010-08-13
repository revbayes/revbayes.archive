/**
 * @file
 * This file contains the declaration of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * @brief Declaration of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef RbString_H
#define RbString_H

#include "RbComplex.h"

#include <string>

class RbDumpState;


class RbString : public RbComplex {

    public:
                                RbString(void) : RbComplex(), value() {}//!< Default: empty string
                                RbString(const std::string& v);         //!< Constructor from string

        // Basic utility functions
	    RbObject*               clone() const;                          //!< Copy
	    bool                    equals(const RbObject* obj) const;      //!< Equals comparison
        const StringVector&     getClass() const;                       //!< Get class
        void                    printValue(std::ostream& o) const;      //!< Print value (for user)
        std::string             toString(void) const;                   //!< General info on object

        // Overloaded operators
        RbString                operator+(const RbString& s) const;     //!< String concatenation
                                operator std::string() const;           //!< Type conversion

        // Regular functions
        void                    append(const RbString& s) { value += s.value; } //!< Append string
        void                    append(const char* s) { value += s; }           //!< Append C string
        std::string             getValue(void) const { return value; }          //!< Get value
        void                    setValue(const std::string& x) { value = x; }   //!< Set value

    private:
        std::string             value;      //!< Value member
};

#endif

