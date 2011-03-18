/**
 * @file DistributionBeta
 * This file contains the functions of the beta distribution.
 *
 * @brief Implementation of the beta distribution.
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */

#include <cmath>

#include "DistributionBeta.h"
#include "RbMathFunctions.h"

/*!
 * This function returns the probability density for a 
 * Beta-distributed random variable.
 *
 * \brief Beta probability density.
 * \param a parameter of the Beta. 
 * \param b parameter of the Beta. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::pdf(double a, double b, double x) {
    
	double pdf;
	if ( x < 0.0 || 1.0 < x )
		pdf = 0.0;
	else
		pdf = std::pow(x, (a - 1.0)) * std::pow((1.0 - x), (b - 1.0)) / RbMath::beta(a, b);
	return pdf;
}

/*!
 * This function returns the natural log of the probability density 
 * for a Beta-distributed random variable.
 *
 * \brief Beta log probability density.
 * \param a parameter of the Beta. 
 * \param b parameter of the Beta. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::lnPdf(double a, double b, double x) {
    
	return ( (RbMath::lnGamma(a + b) - RbMath::lnGamma(a) - RbMath::lnGamma(b)) + (a - 1.0) * std::log(x) + (b - 1.0) * std::log(1.0 - x) );
}

/*!
 * This function returns the cumulative probability for a 
 * Beta-distributed random variable.
 *
 * \brief Beta cumulative probability.
 * \param a parameter of the Beta. 
 * \param b parameter of the Beta. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::cdf(double a, double b, double x) {
    
	double cdf;
	if ( x <= 0.0 )
		cdf = 0.0;
	else if ( x <= 1.0 )
		cdf = RbMath::incompleteBeta(a, b, x);
	else
		cdf = 1.0;
	return cdf;
}

/*!
 * This function returns the quantile for a 
 * Beta-distributed random variable.
 *
 * \brief Beta quantile.
 * \param a parameter of the Beta. 
 * \param b parameter of the Beta. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::quantile(double a, double b, double p) {
    
	double bcoeff;
	double error = 0.0001;
	double errapp = 0.01;
	int j;
    
	/* estimate the solution */
	double x = a / ( a + b );
    
	double xOld = 0.0;
	int loopCnt = 2;
	double d[MAXK * (MAXK-1)];
	while ( errapp <= fabs ( ( x - xOld ) / x ) && loopCnt != 0 )
    {
		xOld = x;
		loopCnt--;
		/* cdfX = PROB { BETA(A,B) <= X }
         q = ( cdf - cdfX ) / pdfX */
		double cdfX = Beta::cdf(a, b, x);
		double pdfX = Beta::pdf(a, b, x);
		double q = (p - cdfX) / pdfX;
		/* D(N,K) = C(N,K) * Q**(N+K-1) / (N-1)! */
		double t = 1.0 - x;
		double s1 = q * ( b - 1.0 ) / t;
		double s2 = q * ( 1.0 - a ) / x;
		d[2-1+0*MAXK] = s1 + s2;
		double tail = d[2-1+0*MAXK] * q / 2.0;
		x = x + q + tail;
        
		int k = 3;
		while ( error < fabs ( tail / x ) && k <= MAXK )
        {
			/* find D(2,K-2) */
			s1 = q * ((double)(k) - 2.0) * s1 / t;
			s2 = q * (2.0 - (double)(k)) * s2 / x;
			d[2-1+(k-2)*MAXK] = s1 + s2;
			/* find D(3,K-3), D(4,K-4), D(5,K-5), ... , D(K-1,1) */
			for (int i=3; i<=k-1; i++)
            {
				double sum2 = d[2-1+0*MAXK] * d[i-2+(k-i)*MAXK];
				bcoeff = 1.0;
				for ( j = 1; j <= k - i; j++ )
                {
					bcoeff = ( bcoeff * ( double ) ( k - i - j + 1 ) ) / ( double ) ( j );
					sum2 = sum2 + bcoeff * d[2-1+j*MAXK] * d[i-2+(k-i-j)*MAXK];
                }
				d[i-1+(k-i)*MAXK] = sum2 + d[i-2+(k-i+1)*MAXK] / (double)(i - 1);
            }
			/* compute D(K,0) and use it to expand the series */
			d[k-1+0*MAXK] = d[2-1+0*MAXK] * d[k-2+0*MAXK] + d[k-2+1*MAXK] / (double)(k - 1);
			tail = d[k-1+0*MAXK] * q / (double)(k);
			x += tail;
			/* check for divergence */
			if ( x <= 0.0 || 1.0 <= x )
            {
				std::cout << "Error in betaQuantile: The series has diverged" << std::endl;
				x = -1.0;
				return x;
            }
			k++;
        }
    }
	return x;
}

/*!
 * This function generates a Beta-distributed random variable.
 *
 * \brief Beta random variable.
 * \param a parameter of the Beta. 
 * \param b parameter of the Beta. 
 * \param rng is a pointer to a random number object. 
 * \return Returns the random variable.
 * \throws Does not throw an error.
 */
double RbStatistics::Beta::rv(double a, double b, RandomNumberGenerator* rng) {
    
	double z0 = RbStatistics::Helper::rndGamma( a, *rng );
	double z1 = RbStatistics::Helper::rndGamma( b, *rng );
	double sum = z0 + z1;
	double x = z0 / sum;
	return x;
}
