#include "Simplex.h"
#include <iostream>



Simplex::Simplex(int n) {

	typeName = "Simplex";
	value.resize(n);
	double temp = 1.0 / value.size();
	for (int i=0; i<value.size(); i++)
		value[i] = temp;
}

Simplex::Simplex(Simplex &s) {

	typeName = s.typeName;
	value.resize( s.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = s[i];
}

Simplex::Simplex(std::vector<double> &s) {

	typeName = "Simplex";
	value.resize( s.size() );
	for (int i=0; i<value.size(); i++)
		value[i] = s[i];
}

Simplex* Simplex::copy(void) {

	Simplex *x = new Simplex( *this );
	return x;
}

void Simplex::print(void) {

	std::cout << "(";
	for (int i=0; i<value.size(); i++)
		{
		std::cout << value[i];
		if ( i < value.size()-1 )
			std::cout << ",";
		}
	std::cout << ")" << std::endl;
}