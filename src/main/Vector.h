#ifndef Vector_H
#define Vector_H

#include <vector>
#include "RbDataType.h"

class Vector : public RbDataType {

public:
	Vector(int n);
	Vector(int n, double v);
	Vector(Vector &v);
	Vector(std::vector<double> &v);
	double operator[](int i) { return value[i]; } 
	void print(void);
	Vector* copy(void);
	int size(void) { return value.size(); }
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	std::vector<double> value;
};

#endif