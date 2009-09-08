#include <iostream>
#include <cmath>

#include "../distributions/Distribution.h"
#include "../distributions/DistNormal.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"

int main(int argc, char **argv) {
	// create a DistNormal object
	DistNormal* dist = new DistNormal();

	RbDouble* sigma = new RbDouble(1.0);
	RbDouble* mu = new RbDouble(0.0);
	std::vector<RbDataType*> arguments(2);
	arguments[0] = mu;
	arguments[1] = sigma;

	RbDouble* x = new RbDouble(0.5);

	// TODO some more meaningful test

	// calculate the pdf for some known parameters
	double pdf = dist->pdf(x, arguments);
	double expectedPDF = 0.3520653; //taken from R
//	std::cout << "The pdf is " << pdf << '\n';
//	std::cout << "The expected pdf is " << expectedPDF << '\n';
	assert(abs(pdf-expectedPDF) <= 0.0000001);

	// calculate the lnpdf for some known parameters
	double lnpdf = dist->lnPdf(x, arguments);
	double expectedLnPDF = -1.043939; //taken from R
//	std::cout << "The lnpdf is " << lnpdf << '\n';
//	std::cout << "The expected lnpdf is " << expectedLnPDF << '\n';
	assert(abs(lnpdf-expectedLnPDF) <= 0.000001);

	// calculate the cdf for some known parameters
	double cdf = dist->cdf(x, arguments);
	double expectedCDF = 0.6914625; //taken from R
//	std::cout << "The cdf is " << cdf << '\n';
//	std::cout << "The expected cdf is " << expectedCDF << '\n';
	assert(abs(cdf-expectedCDF) <= 0.0000001);

	// calculate the quantile for some known parameters
	std::vector<RbDataType*> argumentsQ(3);
	argumentsQ[0] = mu;
	argumentsQ[1] = sigma;
	argumentsQ[2] = x;
	double q = dist->quantile(argumentsQ);
	double expectedQ = 0.0; //taken from R
//	std::cout << "The quantile is " << q << '\n';
//	std::cout << "The expected quantile is " << expectedQ << '\n';
	assert(abs(q-expectedQ) <= 0.0000001);

	// draw some random values some known parameters and compare this to the pdf and cdf.

	assert(true);

}
