/*!
 * \file
 * This file contains the implementation of RbStringVector, which
 * holds a vector of strings.
 *
 * \brief Implementation of RbStringVector
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbStringVector.h"
#include <iostream>
#include <vector>

using namespace std;

RbStringVector::RbStringVector(const string& v) 
    : RbDataType("RbString"), value(vector<string>()) {

    value.push_back(v);
}

RbStringVector::RbStringVector(vector<string> v) 
    : RbDataType("RbString"), value(v) {
}

RbStringVector::RbStringVector(const RbStringVector& s) :
    RbDataType(s.typeName), value(s.value) {
}

RbStringVector* RbStringVector::copy(void) const {

	RbStringVector *x = new RbStringVector( *this );
	return x;
}

void RbStringVector::print(void) const {

	for (vector<string>::const_iterator i=value.begin(); i!=value.end(); i++)
        cout << (*i) << endl;
}

void RbStringVector::print(ostream& c) const {

	for (vector<string>::const_iterator i=value.begin(); i!=value.end(); i++)
        c << (*i) << endl;
}

