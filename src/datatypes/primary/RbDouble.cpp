/*
 * RbDouble.cpp
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#include "RbDouble.h"
#include "../RbDataType"
#include <iostream>


RbDouble::RbDouble(const double v)
    : RbDataType("RbDouble"), value(v) {
}


RbDouble::RbDouble(const RbDouble& s)
    : RbDataType(s.typeName), value(s.value) {
}

RbDouble* RbDouble::copy(void) const {

	RbDouble *x = new RbDouble( *this );
	return x;
}

void RbDouble::print(void) const {

	cout << value << endl;
}

void RbDouble::print(ostream &c) const {

	c << value << endl;
}
