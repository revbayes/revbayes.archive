#include "Vector.h"
#include <iostream>



Vector::Vector(int n) : RbDataType("Vector") {

	value.resize(n);
}

Vector::Vector(int n, double v) : RbDataType("Vector") {

	value.resize(n);
	for (int i=0; i<n; i++)
		value[i] = v;
}

Vector::Vector(const Vector& v) : RbDataType("Vector") {

	value.resize( v.value.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = v.value[i];
}

Vector::Vector(std::vector<double> &v) : RbDataType("Vector") {

	value.resize( v.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = v[i];
}

Vector* Vector::copy(void) const {

	Vector *x = new Vector( *this );
	return x;
}

void Vector::print(void) const {

	std::cout << "(";
	for (int i=0; i<value.size(); i++)
		{
		std::cout << value[i];
		if ( i < value.size()-1 )
			std::cout << ",";
		}
	std::cout << ")" << std::endl;
}