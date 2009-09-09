#ifndef RateMatrix_H
#define RateMatrix_H

#include "RbAtomicType.h"
#include <ios>

class RateMatrix : public RbAtomicType {

public:
    RateMatrix() {} 
	RateMatrix(int n);
	RateMatrix(const RateMatrix& r);
	~RateMatrix(void);
	RateMatrix* copy(void) const;
	double *operator[](int i) { return value[i]; } 
	const double *operator[](int i) const { return value[i]; }
	void print(std::ostream& c=std::cout) const;
	int size(void) { return dim; }
    int getSize(void) const { return 1; }
	bool store(void) { return true; }
	bool restore(void) { return true; }

private:
	int dim;
	double **value;
};

#endif
