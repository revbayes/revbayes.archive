/*
 * RbFloat.cpp
 *
 *  Created on: 7 sep 2009
 *      Author: Sebastian
 */

#include "RbFloat.h"

RbFloat::RbFloat(const float v)
    : RbDataType("RbFloat"), value(v) {
}


RbFloat::RbFloat(const RbFloat& s)
    : RbDataType(s.typeName), value(s.value) {
}

RbFloat* RbFloat::copy(void) const {

	RbFloat *x = new RbFloat( *this );
	return x;
}

void RbFloat::print(void) const {

	cout << value << endl;
}

void RbFloat::print(ostream &c) const {

	c << value << endl;
}
