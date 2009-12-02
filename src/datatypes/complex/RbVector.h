#ifndef Vector_H
#define Vector_H

#include <vector>
#include "RbNumeric.h"

class RbVector : public RbNumeric {

public:
        RbVector(int n);
        RbVector(int n, double v);
        RbVector(const Vector& v);
        RbVector(std::vector<double> &v);

	double operator[](int i) { return value[i]; } 
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	std::vector<double> value;
};

#endif
