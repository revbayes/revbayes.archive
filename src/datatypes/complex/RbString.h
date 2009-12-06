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
        static const StringVector   rbClass;            //!< Static class attribute

	        RbString(const std::string& v);      //!< Constructor from string
	        RbString(const RbString& s);    //!< Copy constructor

        // Basic utility functions
	    RbObject*               clone() const;                          //!< Copy
	    bool                    equals(const RbObject* obj) const;      //!< Equals comparison
	    const StringVector&     getClass() const;                       //!< Get class
	    std::string             getString() const { return value; }     //!< Get string
	    void                    printValue(std::ostream& o) const;      //!< Print value (for user)
	    std::string             toString(void) const;                   //!< General info on object

	    // overloaded operators
	    RbObject&               operator=(const RbObject& o);
	    RbString&               operator=(const RbString& o);
	    RbString                operator+(const RbString& s) const;
                                operator std::string() const;

        void                    dump(std::ostream& c);                   //!< Dump to ostream c
        void                    resurrect(const RbDumpState& x);         //!< Resurrect from dumped state

    private:
	    std::string             value;      //!< Value member
};

#endif

