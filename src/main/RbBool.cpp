/*!
 * \file
 * This file contains the implementation of RbBool, which is
 * a REvBayes wrapper around a regular bool.
 *
 * \brief Implementation of RbBool
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "RbBool.h"
#include <iostream>

using namespace std;

RbBool::RbBool(const bool v)
    : RbDataType("RbBool"), value(v) {
}

RbBool::RbBool(const RbBool& s)
    : RbDataType(s.typeName), value(s.value) {
}

RbBool* RbBool::copy(void) const {

	RbBool *x = new RbBool( *this );
	return x;
}

void RbBool::print(void) const {

	cout << value << endl;
}
