#include "Scalar.h"
#include <iostream>



Scalar::Scalar(double v) {

	typeName = "Scalar";
	value = v;
}


Scalar::Scalar(Scalar &s) {

	typeName = s.typeName;
	value = s.value;
}

Scalar* Scalar::copy(void) {

	Scalar *x = new Scalar( *this );
	return x;
}

void Scalar::print(void) {

	std::cout << value << std::endl;
}