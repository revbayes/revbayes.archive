/*!
 * \file
 * This file contains the implementation of Scalar, which is
 * a REvBayes wrapper around a regular double.
 *
 * \brief Implementation of Scalar
 *
 * (c) Copyright 2009-
 * \date Last modified: $Date$
 * \author The REvBayes development core team
 * \license GPL version 3
 *
 * $Id$
 */

#include "Scalar.h"
#include <iostream>


Scalar::Scalar(const double v) : RbDataType("Scalar") {

	value = v;
}


Scalar::Scalar(const Scalar& s) : RbDataType(s.typeName) {

	value = s.value;
}

Scalar* Scalar::copy(void) const {

	Scalar *x = new Scalar( *this );
	return x;
}

void Scalar::print(void) const {

	std::cout << value << std::endl;
}

