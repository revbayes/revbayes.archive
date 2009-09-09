#include "Simplex.h"
#include <iostream>



Simplex::Simplex(int n) : RbAtomicType() {

	value.resize(n);
	double temp = 1.0 / value.size();
	for (size_t i=0; i<value.size(); i++)
		value[i] = temp;
}

Simplex::Simplex(const Simplex &s) : RbAtomicType() {

	value.resize( s.value.size() );
	for (size_t i=0; i<value.size(); i++)
		value[i] = s.value[i];
}

Simplex::Simplex(std::vector<double> &s) : RbAtomicType() {

	value.resize( s.size() );
	for (size_t i=0; i<value.size(); i++)
		value[i] = s[i];
}

Simplex* Simplex::copy(void) const {

	Simplex *x = new Simplex( *this );
	return x;
}

void Simplex::print(std::ostream& c) const {

	c << "(";
	for (size_t i=0; i<value.size(); i++)
		{
		c << value[i];
		if ( i < value.size()-1 )
			c << ",";
		}
	c << ")" << std::endl;
}
