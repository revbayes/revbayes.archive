#include "Distribution.h"
#include "DistDirichlet.h"
#include "RbMath.h"
#include <cmath>
#include <iostream>



void DistDirichlet::drawRandomValue(const std::vector<double> &a, std::vector<double> &z) {

	int n = a.size();
	double sum = 0.0;
	for(int i=0; i<n; i++)
		{
		z[i] = rndGamma(a[i]);
		sum += z[i];
		}
	for(int i=0; i<n; i++)
		z[i] /= sum;
}

double DistDirichlet::pdf(const std::vector<double> &a, const std::vector<double> &z) {
	
	int n = a.size();
	double zSum = 0.0;
	for (int i=0; i<n; i++)
		zSum += z[i];

	double tol = 0.0001;
	if ( tol < fabs( zSum - 1.0 ) )
		{
		std::cerr << "Fatal error in dirichletPdf" << std::endl;
		exit(1);
		}

	double aSum = 0.0;
	for (int i=0; i<n; i++)
		aSum += a[i];

	double aProd = 1.0;
	for (int i=0; i<n; i++)
		aProd *= gamma(a[i]);

	double pdf = gamma(aSum) / aProd;

	for (int i=0; i<n; i++)
		pdf = pdf * pow( z[i], a[i] - 1.0 );

	return pdf;
}

double DistDirichlet::lnPdf(const std::vector<double> &a, const std::vector<double> &z) {

	int n = a.size(); //!< we assume that a and z have the same size
	double alpha0 = 0.0;
	for (int i=0; i<n; i++)
		alpha0 += a[i];
	double lnP = RbMath::lnGamma(alpha0);
	for (int i=0; i<n; i++)
		lnP -= lnGamma(a[i]);
	for (int i=0; i<n; i++)
		lnP += (a[i] - 1.0) * log(z[i]);	
	return lnP;
}
