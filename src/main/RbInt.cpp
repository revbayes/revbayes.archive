/*!
 * \file
 * This file contains the implementation of RbInt, which is
 * a REvBayes wrapper around a regular int.
 *
 * \brief Implementation of RbInt
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbInt.h"
#include <iostream>

using namespace std;

RbInt::RbInt(const int v)
    : RbDataType("RbInt"), value(v) {
}


RbInt::RbInt(const RbInt& s)
    : RbDataType(s.typeName), value(s.value) {
}

RbInt* RbInt::copy(void) const {

	RbInt *x = new RbInt( *this );
	return x;
}

void RbInt::print(void) const {

	cout << value << endl;
}

void RbInt::print(ostream &c) const {

	c << value << endl;
}
