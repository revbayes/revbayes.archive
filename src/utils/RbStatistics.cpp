#include "RbMath.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbNode.h"
#include "RbStatistics.h"
#include "RbUnrootedTopology.h"
#include "RandomNumberGenerator.h"
#include <cmath>
#include <iostream>
#include <vector>

#define MAXK 20

#pragma mark Beta Distribution

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

#pragma mark Chi-Square Distribution

/*!
 * This function calculates the probability density 
 * for a chi-square distributed random variable.
 *
 * \brief Chi-square probability density.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::pdf(double v, double x) {

	double pdf;
	if ( x < 0.0 )
		{
		pdf = 0.0;
		}
	else
		{
		double b = v / 2.0;
		pdf = exp( -0.5 * x ) * pow( x, ( b - 1.0 ) ) / ( pow( 2.0, b ) * RbMath::gamma( b ) );
		}
	return pdf;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a chi-square distributed random variable.
 *
 * \brief Natural log of chi-square probability density.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::lnPdf(double v, double x) {

	double b = v / 2.0;
	return ( -(b * log(2.0) + RbMath::lnGamma(b)) - b + (b - 1.0) * std::log(x) );
}

/*!
 * This function calculates the cumulative probability  
 * for a chi-square distributed random variable.
 *
 * \brief Chi-square cumulative probability.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::cdf(double v, double x) {

	return RbStatistics::Gamma::cdf( v / 2.0, 0.5, x );
}

/*!
 * This function calculates the quantile of a chi square distribution with v
 * degrees of freedom.
 *
 * \brief Quantile of a chi square distribution.
 * \param v is the degrees of freedom of the chi square. 
 * \param prob is the probability up to the quantile. 
 * \return Returns quantile value (or -1 if in error). 
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::quantile(double v, double prob) {
// @TODO remove all goto statements!!!
	double 		e = 0.5e-6, aa = 0.6931471805, p = prob,
					a = 0.0, q = 0.0, p1 = 0.0, p2 = 0.0, t = 0.0, 
					x = 0.0, b = 0.0;

	if (p < 0.000002 || p > 0.999998 || v <= 0.0) 
		return (-1.0);
	double g = RbMath::lnGamma(v/2.0);
	double xx = v/2.0;   
	double c = xx - 1.0;
	double ch;
	if (v >= -1.24*log(p)) 
		goto l1;
	ch = std::pow((p*xx*exp(g+xx*aa)), 1.0/xx);
	if (ch-e < 0) 
		return (ch);
	goto l4;
	l1:
		if (v > 0.32) 
			goto l3;
		ch = 0.4;   
		a = std::log(1.0-p);
	l2:
		q = ch;  
		p1 = 1.0+ch*(4.67+ch);  
		p2 = ch*(6.73+ch*(6.66+ch));
		t = -0.5+(4.67+2.0*ch)/p1 - (6.73+ch*(13.32+3.0*ch))/p2;
		ch -= (1.0-exp(a+g+0.5*ch+c*aa)*p2/p1)/t;
		if (fabs(q/ch-1.0)-0.01 <= 0.0) 
			goto l4;
		else                       
			goto l2;
	l3: 
		x = RbStatistics::Normal::quantile(p);
		p1 = 0.222222/v;   
		ch = v*pow((x*sqrt(p1)+1.0-p1), 3.0);
		if (ch > 2.2*v+6.0)  
			ch = -2.0*(log(1.0-p)-c*log(0.5*ch)+g);
	l4:
		q = ch;   
		p1 = 0.5*ch;
		if ( (t = RbMath::incompleteGamma(p1, xx, g)) < 0.0 ) 
			{
		    // @TODO throw an actual error
			std::cerr << "Error in function \"IncompleteGamma" << std::endl;
			return (-1.0);
			}
		p2 = p-t;
		t = p2*exp(xx*aa+g+p1-c*log(ch));   
		b = t/ch;  
		a = 0.5*t-b*c;
		double s1 = (210.0+a*(140.0+a*(105.0+a*(84.0+a*(70.0+60.0*a))))) / 420.0;
		double s2 = (420.0+a*(735.0+a*(966.0+a*(1141.0+1278.0*a))))/2520.0;
		double s3 = (210.0+a*(462.0+a*(707.0+932.0*a)))/2520.0;
		double s4 = (252.0+a*(672.0+1182.0*a)+c*(294.0+a*(889.0+1740.0*a)))/5040.0;
		double s5 = (84.0+264.0*a+c*(175.0+606.0*a))/2520.0;
		double s6 = (120.0+c*(346.0+127.0*c))/5040.0;
		ch += t*(1+0.5*t*s1-b*c*(s1-b*(s2-b*(s3-b*(s4-b*(s5-b*s6))))));
		if (fabs(q/ch-1.0) > e) 
			goto l4;
	return (ch);
}

#pragma mark Dirichlet Distribution

/*!
 * This function calculates the probability density 
 * for a Dirichlet-distributed random variable.
 *
 * \brief Dirichlet probability density.
 * \param *a is a pointer to a vector of doubles containing the Dirichlet parameters. 
 * \param *z is a pointer to a vector of doubles containing the random variables. 
 * \param n is the number of Dirichlet parameters/random variables.
 * \return Returns the probability density.
 * \throws Throws an MbException::ERROR.
 */
double RbStatistics::Dirichlet::pdf(const std::vector<double> &a, const std::vector<double> &z) {
	
	int n = a.size();
	double zSum = 0.0;
	for (int i=0; i<n; i++)
		zSum += z[i];

	double tol = 0.0001;
	if ( tol < fabs( zSum - 1.0 ) )
		{
		std::cerr << "ERROR: Fatal error in dirichletPdf" << std::endl;
		exit(1);
		}

	double aSum = 0.0;
	for (int i=0; i<n; i++)
		aSum += a[i];

	double aProd = 1.0;
	for (int i=0; i<n; i++)
		aProd *= RbMath::gamma(a[i]);

	double pdf = RbMath::gamma(aSum) / aProd;

	for (int i=0; i<n; i++)
		pdf = pdf * pow( z[i], a[i] - 1.0 );

	return pdf;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a Dirichlet-distributed random variable.
 *
 * \brief Natural log of Dirichlet probability density.
 * \param *a is a pointer to a vector of doubles containing the Dirichlet parameters. 
 * \param *z is a pointer to a vector of doubles containing the random variables. 
 * \param n is the number of Dirichlet parameters/random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Dirichlet::lnPdf(const std::vector<double> &a, const std::vector<double> &z) {

	int n = a.size(); //!< we assume that a and z have the same size
	double alpha0 = 0.0;
	for (int i=0; i<n; i++)
		alpha0 += a[i];
	double lnP = RbMath::lnGamma(alpha0);
	for (int i=0; i<n; i++)
		lnP -= RbMath::lnGamma(a[i]);
	for (int i=0; i<n; i++)
		lnP += (a[i] - 1.0) * std::log(z[i]);	
	return lnP;
}

#pragma mark Exponential Distribution

/*!
 * This function calculates the probability density 
 * for an exponentially-distributed random variable.
 *
 * \brief Exponential probability density.
 * \param lambda is the rate parameter of the exponential. 
 * \param x is the exponential random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Exponential::pdf(double lambda, double x) {

	return lambda * exp(-lambda * x);
}

/*!
 * This function calculates the natural log of the probability density 
 * for an exponentially-distributed random variable.
 *
 * \brief Natural log of exponential probability density.
 * \param lambda is the rate parameter of the exponential. 
 * \param x is the exponential random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Exponential::lnPdf(double lambda, double x) {

	return (std::log(lambda) - lambda * x);
}

/*!
 * This function calculates the cumulative probability  
 * for an exponentially-distributed random variable.
 *
 * \brief Exponential cumulative probability.
 * \param lambda is the rate parameter of the exponential. 
 * \param x is the exponential random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Exponential::cdf(double lambda, double x) {

	return 1.0 - exp(-lambda * x);
}

/*!
 * This function returns the quantile of a exponential probability 
 * distribution.
 *
 * \brief Exponential quantile.
 * \param lambda is the rate parameter. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Exponential::quantile(double lambda, double p) {

	return -(1.0 / lambda) * std::log(1.0 - p);
}

double RbStatistics::Exponential::rv(RandomNumberGenerator* rng, double lambda) {

	double u = rng->nextDouble();
	return -(1.0/lambda) * log(u);
}

#pragma mark Gamma Distribution

/*!
 * This function calculates the probability density 
 * for a gamma-distributed random variable.
 *
 * \brief Gamma probability density.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::pdf(double a, double b, double x) {

	return (pow(b, a) / RbMath::gamma(a)) * pow(x, a - 1.0) * exp(-x * b);
}

/*!
 * This function calculates the natural log of the probability density 
 * for a gamma-distributed random variable.
 *
 * \brief Natural log of gamma probability density.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::lnPdf(double a, double b, double x) {

	return a * log(b) - RbMath::lnGamma(a) + (a - 1.0) * log(x) - x * b;
}

/*!
 * This function calculates the cumulative probability  
 * for a gamma-distributed random variable.
 *
 * \brief Gamma cumulative probability.
 * \param a is the shape parameter of the gamma. 
 * \param b is the scale parameter of the gamma. 
 * \param x is the gamma random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::cdf(double a, double b, double x) {

	return RbMath::incompleteGamma( b*x, a, RbMath::lnGamma(a) );
}

/*!
 * This function returns the quantile of a gamma probability 
 * distribution.
 *
 * \brief Gamma quantile.
 * \param a is the shape parameter. 
 * \param b is the scale parameter. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Gamma::quantile(double a, double b, double p) {

	return ChiSquare::quantile(p, 2.0 * a) / (2.0*b);
}

#pragma mark Log Normal Distribution

/*!
 * This function calculates the probability for a
 * Poisson distribution. 
 *
 * \brief Poisson probability.
 * \param lambda is the rate parameter of the Poisson. 
 * \param x is the value of the random variable. 
 * \return Returns the probability. 
 * \throws Does not throw an error.
 */
/*!
 * This function calculates the probability density 
 * for a log normally distributed random variable.
 *
 * \brief Log normal probability density.
 * \param mu is the mean parameter of the log normal. 
 * \param sigma is the variance parameter of the log normal. 
 * \param x is the log normal random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::LogNormal::pdf(double mu, double sigma, double x) {

	double pdf;
	if ( x <= 0.0 )
		{
		pdf = 0.0;
		}
	else
		{
		double y = ( std::log(x) - mu ) / sigma;
		pdf = exp( -0.5 * y * y ) / ( sigma * x * sqrt(2.0 * PI) );
		}
	return pdf;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a log normally distributed random variable.
 *
 * \brief Natural log of the log normal probability density.
 * \param mu is the mean parameter of the log normal. 
 * \param sigma is the variance parameter of the log normal. 
 * \param x is the log normal random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::LogNormal::lnPdf(double mu, double sigma, double x) {

	return ( - 0.5 * ( (std::log(x) - mu) / sigma ) * ( (std::log(x) - mu) / sigma ) ) - std::log( sigma * x * sqrt( 2.0 * PI ) );
}

/*!
 * This function calculates the cumulative probability 
 * for a log normally distributed random variable.
 *
 * \brief Log normal cumulative probability.
 * \param mu is the mean parameter of the log normal. 
 * \param sigma is the variance parameter of the log normal. 
 * \param x is the log normal random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::LogNormal::cdf(double mu, double sigma, double x) {

	double cdf;
	if ( x <= 0.0 )
		{
		cdf = 0.0;
		}
	else
		{
		double logX = std::log(x);
		cdf = RbStatistics::Normal::cdf( mu, sigma, logX );
		}
	return cdf;
}

/*!
 * This function returns the quantile of a normal probability 
 * distribution.
 *
 * \brief Natural quantile.
 * \param mu is the mean parameter of the normal. 
 * \param sigma is the variance parameter of the normal. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::LogNormal::quantile(double mu, double sigma, double p) {

  return exp( RbStatistics::Normal::quantile(mu, sigma, p) );
}

#pragma mark Poisson Distribution

double RbStatistics::Poisson::pdf(double lambda, int x) {

	return exp(x * std::log(lambda) - lambda - RbMath::lnFactorial(x));
}

/*!
 * This function calculates the natural log of the probability for a
 * Poisson distribution. 
 *
 * \brief Natural log of Poisson probability.
 * \param lambda is the rate parameter of the Poisson. 
 * \param x is the value of the random variable. 
 * \return Returns the natural log of the probability. 
 * \throws Does not throw an error.
 */
double RbStatistics::Poisson::lnPdf(double lambda, int x) {

	return ( x * std::log(lambda) - lambda - RbMath::lnFactorial(x) );
}

/*!
 * This function calculates the cumulative probability for a
 * Poisson distribution. 
 *
 * \brief Poisson cumulative probability.
 * \param lambda is the rate parameter of the Poisson. 
 * \param x is the value of the random variable. 
 * \return Returns the cumulative probability. 
 * \throws Does not throw an error.
 */
double RbStatistics::Poisson::cdf(double lambda, int x) {

	if ( x < 0 )
		return 0.0;
	double next = exp(-lambda);
	double cdf = next;
	for (int i=1; i<=x; i++)
		{
		double last = next;
		next = last * lambda / (double)i;
		cdf += next;
		}
	return cdf;
}

/*!
 * This function returns the quantile of a Poisson probability 
 * distribution.
 *
 * \brief Poisson(lambda) quantile.
 * \param lambda is the rate parameter of the Poisson. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Poisson::quantile(double lambda, double p) {

	/* Starting with x = 0, find the first value for which
	   CDF(X-1) <= CDF <= CDF(X). */
	double sum = 0.0;
	int xmax = 100;
	for (int i=0; i<=xmax; i++)
		{
		double sumOld = sum;
		double newVal;
		if ( i == 0 )
			{
			newVal = exp(-lambda);
			sum = newVal;
			}
		else
			{
			double last = newVal;
			newVal = last * lambda / ( double ) ( i );
			sum += newVal;
			}
		if ( sumOld <= p && p <= sum )
			return i;
		}
	//cout << "Poisson quantile warning" << endl;
	return xmax;
}

#pragma mark Normal Distribution

/*!
 * This function calculates the probability density 
 * for a normally-distributed random variable.
 *
 * \brief Normal probability density.
 * \param mu is the mean parameter of the normal. 
 * \param sigma is the variance parameter of the normal. 
 * \param x is the normal random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Normal::pdf(double mu, double sigma, double x) {

	double y = ( x - mu ) / sigma;
	return exp( -0.5 * y * y )  / ( sigma * sqrt ( 2.0 * PI ) );
}

/*!
 * This function calculates the natural log of the probability density 
 * for a normally-distributed random variable.
 *
 * \brief Natural log of normal probability density.
 * \param mu is the mean parameter of the normal. 
 * \param sigma is the variance parameter of the normal. 
 * \param x is the normal random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Normal::lnPdf(double mu, double sigma, double x) {

	return -0.5 * std::log(2.0 * PI * sigma) - 0.5 * (x - mu) * (x - mu) / (sigma * sigma);
}

/*!
 * This function calculates the cumulative probability 
 * for a normally-distributed random variable.
 *
 * \brief Normal cumulative probability.
 * \param mu is the mean parameter of the normal. 
 * \param sigma is the variance parameter of the normal. 
 * \param x is the normal random variable. 
 * \return Returns the cumulative probability.
 * \see Adams, A. G. 1969. Areas under the normal curve. Cojputer J. 12:197-198.
 * \throws Does not throw an error.
 */
double RbStatistics::Normal::cdf(double mu, double sigma, double x) {

	double cdf;
	double q;
	double z = (x - mu) / sigma;

	/* |X| <= 1.28 */
	if ( fabs(z) <= 1.28 )
		{
		double a1 = 0.398942280444;
		double a2 = 0.399903438504;
		double a3 = 5.75885480458;
		double a4 = 29.8213557808;
		double a5 = 2.62433121679;
		double a6 = 48.6959930692;
		double a7 = 5.92885724438;
		double y = 0.5 * z * z;
		q = 0.5 - fabs(z) * ( a1 - a2 * y / ( y + a3 - a4 / ( y + a5 + a6 / ( y + a7 ) ) ) );
		}
	else if ( fabs(z) <= 12.7 )
		{
		double b0 = 0.398942280385;
		double b1 = 3.8052E-08;
		double b2 = 1.00000615302;
		double b3 = 3.98064794E-04;
		double b4 = 1.98615381364;
		double b5 = 0.151679116635;
		double b6 = 5.29330324926;
		double b7 = 4.8385912808;
		double b8 = 15.1508972451;
		double b9 = 0.742380924027;
		double b10 = 30.789933034;
		double b11 = 3.99019417011;
		double y = 0.5 * z * z;
		q = exp(-y) * b0 / (fabs(z) - b1 + b2 / (fabs(z) + b3 + b4 / (fabs(z) - b5 + b6 / (fabs(z) + b7 - b8 / (fabs(z) + b9 + b10 / (fabs(z) + b11))))));
		}
	else
		{
		q = 0.0;
		}
	if ( z < 0.0 )
		{
		/* negative x */
		cdf = q;
		}
	else
		{
		/* positive x */
		cdf = 1.0 - q;
		}
	return cdf;
}

/*!
 * This function calculates the quantiles of a standard normal distribution.
 *
 * \brief Quantile of a standard normal distribution.
 * \param prob is the probability up to the quantile. 
 * \return Returns quantile value. 
 * \throws Does not throw an error.
 * \see Odeh, R. E. and J. O. Evans. 1974. The percentage points of the normal
 *      distribution. Applied Statistics, 22:96-97.
 * \see Wichura, M. J.  1988. Algorithm AS 241: The percentage points of the
 *      normal distribution. 37:477-484.
 * \see Beasley, JD & S. G. Springer. 1977. Algorithm AS 111: The percentage
 *      points of the normal distribution. 26:118-121.
 */
double RbStatistics::Normal::quantile(double p) {

	double a0 = -0.322232431088;
	double a1 = -1.0;
	double a2 = -0.342242088547;
	double a3 = -0.0204231210245;
 	double a4 = -0.453642210148e-4;
 	double b0 = 0.0993484626060;
 	double b1 = 0.588581570495;
 	double b2 = 0.531103462366; 
 	double b3 = 0.103537752850; 
 	double b4 = 0.0038560700634;
	double p1 = ( p < 0.5 ? p : 1.0 - p);
	if (p1 < 1e-20) 
	   return (-9999.0);
	double y = sqrt( log(1.0/(p1*p1)) );   
	double z = y + ((((y*a4+a3)*y+a2)*y+a1)*y+a0) / ((((y*b4+b3)*y+b2)*y+b1)*y+b0);
	return ( p < 0.5 ? -z : z );
}

/*!
 * This function returns the quantile of a normal probability 
 * distribution.
 *
 * \brief Natural quantile.
 * \param mu is the mean parameter of the normal. 
 * \param sigma is the variance parameter of the normal. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Normal::quantile(double mu, double sigma, double p) {
	
	double z = Normal::quantile(p);
	double x = z * sigma + mu;
	return x;
}

double RbStatistics::Normal::rv(RandomNumberGenerator* rng, double mu, double sigma) {

	double v1 = 0.0;
	double v2 = 0.0; // NOTE: We should eventually implement this so you generate and
	                 // return the extra normal random variable that is generated
	double rsq = 0.0;
	do
		{
		v1 = 2.0 * rng->nextDouble() - 1.0;
		v2 = 2.0 * rng->nextDouble() - 1.0;
		rsq = v1 * v1 + v2 * v2;
		} while ( rsq >= 1.0 || rsq == 0.0 );
	double fac = sqrt(-2.0 * log(rsq)/rsq);
	//extraNormalRv = v1 * fac;
	//availableNormalRv = true;
	return ( mu + sigma * (v2 * fac) );
}

#pragma mark Uniform(0,1)

/*!
 * This function calculates the probability density 
 * for a uniform(0,1) random variable.
 *
 * \brief Uniform(0,1) probability density.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform01::pdf(double x) {

	if ( x < 0.0 || x > 1.0)
		return 0.0;
	return 1.0;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a uniform(0,1) random variable.
 *
 * \brief Natural log of uniform(0,1) probability density.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform01::lnPdf(double x) {

	if ( x < 0.0 || x > 1.0)
		return RbConstants::Double::neginf;
	return 0.0;
}

/*!
 * This function calculates the cumulative probability  
 * for a uniform(0,1) random variable.
 *
 * \brief Uniform(0,1) cumulative probability.
 * \param x is the uniform random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform01::cdf(double x) {

	if ( x < 0.0 )
		return 0.0;
	else if ( x > 1.0 )
		return 1.0;
	else
		return x;
}

/*!
 * This function returns the quantile of a uniform(0,1) probability 
 * distribution.
 *
 * \brief Uniform(0,1) quantile.
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform01::quantile(double p) {

	return p;
}

#pragma mark Uniform Distribution

/*!
 * This function calculates the probability density 
 * for a uniform(a,b) random variable.
 *
 * \brief Uniform(a,b) probability density.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform::pdf(double a, double b, double x) {

	if ( x < a || x > b)
		return 0.0;
	return 1.0 / (b-a);
}

/*!
 * This function calculates the natural log of the probability density 
 * for a uniform(a,b) random variable.
 *
 * \brief Natural log of uniform(a,b) probability density.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform::lnPdf(double a, double b, double x) {

	if ( x < a || x > b)
		return RbConstants::Double::neginf;
	return ( -std::log(b-a) );
}

/*!
 * This function calculates the cumulative probability 
 * for a uniform(a,b) random variable.
 *
 * \brief Uniform(a,b) cumulative probability.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \param x is the uniform random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform::cdf(double a, double b, double x) {

	if ( x < a )
		return 0.0;
	else if ( x > b )
		return 1.0;
	else
		return (x-a) / (b-a);
}

/*!
 * This function returns the quantile of a uniform(a,b) probability 
 * distribution.
 *
 * \brief Uniform(a,b) quantile.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \param p is the probability up to the quantile. 
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::Uniform::quantile(double a, double b, double p) {

	return a + (b - a) * p;
}

double RbStatistics::Uniform::rv(RandomNumberGenerator* rng, double a, double b) {

	double u = rng->nextDouble();
	return (a + (b-a)*u);
}

#pragma mark Uniform Unrooted Topology Distribution

/*!
 * This function calculates the probability density 
 * for a uniform(a,b) random variable.
 *
 * \brief Uniform(a,b) probability density.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::UniformUnrootedTopology::pdf(int n, RbUnrootedTopology* x) {

	return 1.0;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a uniform(a,b) random variable.
 *
 * \brief Natural log of uniform(a,b) probability density.
 * \param a is the lower bound on the uniform. 
 * \param b is the upper bound on the uniform. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::UniformUnrootedTopology::lnPdf(int n, RbUnrootedTopology* x) {

	return 0.0;
}


RbUnrootedTopology* RbStatistics::UniformUnrootedTopology::rv(RandomNumberGenerator* rng, int n) {

		if (n <= 1)
			{
			throw RbException("The tree is too small to bother to construct");
			}
			
		// construct unrooted tree of three tips
		int nextIntNode = n;
		RbNode* rn = new RbNode(nextIntNode++);
		std::vector<RbNode*> availableNodes;
		int minNumTaxa = 3;
		if (n < minNumTaxa)
			minNumTaxa = n;
		for (int i=0; i<minNumTaxa; i++)
			{
			RbNode *p = new RbNode(i);
			p->setParent(rn);
			rn->addChild(p);
			availableNodes.push_back( p );
			}

	// add the remaining tips
	for (int i=4; i<=n; i++)
		{
		int u = rng->nextInt( availableNodes.size() );
		RbNode* p = availableNodes[u];
		RbNode* r = p->getParent();
		RbNode* newTip = new RbNode(i);
		RbNode* newInt = new RbNode(nextIntNode++);
		r->removeChild( p );
		r->addChild( newInt );
		newInt->setParent( r );
		newInt->addChild( p );
		newInt->addChild( newTip );
		p->setParent( newInt );
		newTip->setParent( newInt );
		availableNodes.push_back( newInt );
		availableNodes.push_back( newTip );
		}

		// construct the topology object
	RbUnrootedTopology* t = new RbUnrootedTopology(rn);
			
	return t;
}

#undef MAXK



