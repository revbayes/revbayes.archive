/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>

#include "DistNormal.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"
#include "../utils/RbMath.h"

DistNormal::DistNormal() {
	// TODO Auto-generated constructor stub

}

DistNormal::~DistNormal() {
	// TODO Auto-generated destructor stub
}

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
double DistNormal::pdf(RbDataType* variable, std::vector<RbDataType*> arguments) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	assert(arguments.size()==2);
	RbDataType* first = arguments[0];
	assert(typeid(*first) == typeid(RbDouble));
	double mu = ((RbDouble*)first)->getValue();

	RbDataType* second = arguments[1];
	assert(typeid(*second) == typeid(RbDouble));
	double sigma = ((RbDouble*)second)->getValue();

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
double DistNormal::lnPdf(RbDataType* variable, std::vector<RbDataType*> arguments) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	assert(arguments.size()==2);
	RbDataType* first = arguments[0];
	assert(typeid(*first) == typeid(RbDouble));
	double mu = ((RbDouble*)first)->getValue();

	RbDataType* second = arguments[1];
	assert(typeid(*second) == typeid(RbDouble));
	double sigma = ((RbDouble*)second)->getValue();

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
double DistNormal::cdf(RbDataType* variable, std::vector<RbDataType*> arguments) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	assert(arguments.size()==2);
	RbDataType* first = arguments[0];
	assert(typeid(*first) == typeid(RbDouble));
	double mu = ((RbDouble*)first)->getValue();

	RbDataType* second = arguments[1];
	assert(typeid(*second) == typeid(RbDouble));
	double sigma = ((RbDouble*)second)->getValue();

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
 * This function calculates the quantiles of a normal distribution.
 *
 * \brief Quantile of a standard normal distribution.
 * \param the arguments of the distribution.
 * \return Returns quantile value.
 * \throws Does not throw an error.
 * \see Odeh, R. E. and J. O. Evans. 1974. The percentage points of the normal
 *      distribution. Applied Statistics, 22:96-97.
 * \see Wichura, M. J.  1988. Algorithm AS 241: The percentage points of the
 *      normal distribution. 37:477-484.
 * \see Beasley, JD & S. G. Springer. 1977. Algorithm AS 111: The percentage
 *      points of the normal distribution. 26:118-121.
 */
double DistNormal::quantile(std::vector<RbDataType*> arguments) {
	if (arguments.size()==1){
		RbDataType* first = arguments[0];
		assert(typeid(*first) == typeid(RbDouble));
		double p = ((RbDouble*)first)->getValue();
		return quantile(p);
	}
	else if (arguments.size()==3){
		RbDataType* first = arguments[0];
		assert(typeid(*first) == typeid(RbDouble));
		double mu = ((RbDouble*)first)->getValue();

		RbDataType* second = arguments[1];
		assert(typeid(*second) == typeid(RbDouble));
		double sigma = ((RbDouble*)second)->getValue();

		RbDataType* third = arguments[2];
		assert(typeid(*third) == typeid(RbDouble));
		double p = ((RbDouble*)third)->getValue();
		return quantile(mu, sigma, p);
	}
	else {
		// wrong number of arguments
		// TODO throw an error message
		std::cerr << "Wrong number of argument for DistNormal::quantile with " << arguments.size() << " number of arguments." << '\n';
		return 0;
	}
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
double DistNormal::quantile(double p) {

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
double DistNormal::quantile(double mu, double sigma, double p) {

	double z = quantile(p);
	double x = z * sigma + mu;
	return x;
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
RbDataType* DistNormal::rv(void) {
	//TODO implement this

	return NULL;
}


RbDataType DistNormal::getDataType() {
	//TODO implement this
	return RbDouble(0);
}
