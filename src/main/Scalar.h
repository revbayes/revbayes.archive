#ifndef Scalar_H
#define Scalar_H

#include <vector>
#include "RbDataType.h"

class Scalar : public RbDataType {

public:
	Scalar(double v);
	Scalar(Scalar &s);
	void print(void);
	Scalar* copy(void);
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	double value;
};

#endif
