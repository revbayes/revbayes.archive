#ifndef RateMatrix_H
#define RateMatrix_H

#include "RbDataType.h"

class RateMatrix : public RbDataType {

public:
	RateMatrix(int n);
	RateMatrix(const RateMatrix& r);
	~RateMatrix(void);
	RateMatrix* copy(void) const;
	double *operator[](int i) { return value[i]; } 
	const double *operator[](int i) const { return value[i]; }
	void print(void) const;
	int size(void) { return dim; }
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	int dim;
	double **value;
};

#endif