#include "RateMatrix.h"
#include "RbHelper.h"
#include <iomanip>
#include <iostream>



RateMatrix::RateMatrix(int n) : RbDataType("RateMatrix") {

	dim = n;
	value = RbHelper::newTwoDArray<double>(dim, dim);
	for (int i=0; i<dim; i++)
		for (int j=0; j<dim; j++)
			value[i][j] = 0.0;
}

RateMatrix::RateMatrix(const RateMatrix& r) : RbDataType("RateMatrix") {

	dim = r.dim;
	value = RbHelper::newTwoDArray<double>(dim, dim);
	for (int i=0; i<dim; i++)
		for (int j=0; j<dim; j++)
			value[i][j] = r[i][j];
}

RateMatrix::~RateMatrix(void) {

	RbHelper::deleteTwoDArray( value );
}

RateMatrix* RateMatrix::copy(void) const {

	RateMatrix *x = new RateMatrix( *this );
	return x;
}

void RateMatrix::print(void) const {

	for (int i=0; i<dim; i++)
		{
		for (int j=0; j<dim; j++)
			std::cout << std::fixed << std::setprecision(5) << value[i][j] << " ";
		std::cout << std::endl;
		}
}
