#include <iostream>
#include <cmath>

#include "../distributions/Distribution.h"
#include "../distributions/statistics/DistNormal.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"

int main(int argc, char **argv) {
	// create a DistNormal object
	double sigma = 1.0;
	double mu = 0.0;

	DistNormal* dist = new DistNormal(&sigma,&mu);

	RbDouble* x = new RbDouble(0.5);

	// TODO some more meaningful test

	// calculate the pdf for some known parameters
	double pdf = dist->pdf(x);
	double expectedPDF = 0.3520653; //taken from R
//	std::cout << "The pdf is " << pdf << '\n';
//	std::cout << "The expected pdf is " << expectedPDF << '\n';
	assert(abs(pdf-expectedPDF) <= 0.0000001);

	// calculate the lnpdf for some known parameters
	double lnpdf = dist->lnPdf(x);
	double expectedLnPDF = -1.043939; //taken from R
//	std::cout << "The lnpdf is " << lnpdf << '\n';
//	std::cout << "The expected lnpdf is " << expectedLnPDF << '\n';
	assert(abs(lnpdf-expectedLnPDF) <= 0.000001);

	// calculate the cdf for some known parameters
	double cdf = dist->cdf(x);
	double expectedCDF = 0.6914625; //taken from R
//	std::cout << "The cdf is " << cdf << '\n';
//	std::cout << "The expected cdf is " << expectedCDF << '\n';
	assert(abs(cdf-expectedCDF) <= 0.0000001);

	// calculate the quantile for some known parameters
	double q = dist->quantile(x);
	double expectedQ = 0.0; //taken from R
//	std::cout << "The quantile is " << q << '\n';
//	std::cout << "The expected quantile is " << expectedQ << '\n';
	assert(abs(q-expectedQ) <= 0.0000001);

	// draw some random values some known parameters and compare this to the pdf and cdf.

	assert(true);

}
