#include "Simplex.h"
#include <iostream>



Simplex::Simplex(int n) : RbDataType("Simplex") {

	value.resize(n);
	double temp = 1.0 / value.size();
	for (int i=0; i<value.size(); i++)
		value[i] = temp;
}

Simplex::Simplex(const Simplex &s) : RbDataType(s.typeName) {

	value.resize( s.value.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = s.value[i];
}

Simplex::Simplex(std::vector<double> &s) : RbDataType("Simplex") {

	value.resize( s.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = s[i];
}

Simplex* Simplex::copy(void) const {

	Simplex *x = new Simplex( *this );
	return x;
}

void Simplex::print(void) const {

	std::cout << "(";
	for (int i=0; i<value.size(); i++)
		{
		std::cout << value[i];
		if ( i < value.size()-1 )
			std::cout << ",";
		}
	std::cout << ")" << std::endl;
}