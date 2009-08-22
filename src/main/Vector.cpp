#include "Vector.h"
#include <iostream>



Vector::Vector(int n) {

	value.resize(n);
}

Vector::Vector(int n, double v) {

	typeName = "Vector";
	value.resize(n);
	for (int i=0; i<n; i++)
		value[i] = v;
}

Vector::Vector(Vector &v) {

	typeName = v.typeName;
	value.resize( v.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = v[i];
}

Vector::Vector(std::vector<double> &v) {

	typeName = "Vector";
	value.resize( v.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = v[i];
}

Vector* Vector::copy(void) {

	Vector *x = new Vector( *this );
	return x;
}

void Vector::print(void) {

	std::cout << "(";
	for (int i=0; i<value.size(); i++)
		{
		std::cout << value[i];
		if ( i < value.size()-1 )
			std::cout << ",";
		}
	std::cout << ")" << std::endl;
}