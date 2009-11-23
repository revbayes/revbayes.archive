#ifndef Vector_H
#define Vector_H

#include <vector>
#include "RbComplex.h"

class Vector : public RbComplex {

public:
	Vector(int n);
	Vector(int n, double v);
	Vector(const Vector& v);
	Vector(std::vector<double> &v);
	double operator[](int i) { return value[i]; } 
	void print(void) const;
	Vector* copy(void) const;
	int size(void) { return value.size(); }
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	std::vector<double> value;
};

#endif