/*!
 * \file
 * This file contains the implementation of RbString, which is
 * a REvBayes wrapper around a regular string.
 *
 * \brief Implementation of RbString
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbString.h"
#include <iostream>

using namespace std;

RbString::RbString(const string& v) 
    : RbDataType("RbString"), value(v) {
}

RbString::RbString(const RbString& s) :
    RbDataType(s.typeName), value(s.value) {
}

RbString* RbString::copy(void) const {

	RbString *x = new RbString( *this );
	return x;
}

void RbString::print(void) const {

	cout << value << endl;
}

void RbString::print(ostream &c) const {

	c << value << endl;
}
