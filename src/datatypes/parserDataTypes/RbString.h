/**
 * @file
 * This file contains the declaration of RbString, which is
 * a REvBayes wrapper around a vector of strings.
 *
 * @brief Declaration of RbString
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @implements RbDataType, RbAbstractDataType
 * @since Version 1.0, 2009-09-09
 *
 * $Id$
 */

#ifndef RbString_H
#define RbString_H

#include "RbAbstractDataType.h"
#include <ios>
#include <string>
#include <vector>

using namespace std;

class RbString : public RbAbstractDataType, public std::vector<string> {

    public:
        RbString() : RbAbstractDataType("RbString"), vector<string>() {}
	    RbString(const string &v) : RbAbstractDataType("RbString"), vector<string>(1, v) {} //!< Construct from string
	    RbString(const vector<string> &v) : RbAbstractDataType("RbString"), vector<string>(v) {} //!< Construct from vector<string>
        RbString(const RbString& s);    //!< Copy constructor

	RbString*   copy(void) const;                       //!< Copy
    int         getSize(void) const { return size(); }  //!< Get vector size
	void        print(ostream &c=cout) const;           //!< Print the value to ostream c
};

#endif
