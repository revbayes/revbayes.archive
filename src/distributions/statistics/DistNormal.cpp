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

DistNormal::DistNormal(double* s, double* m) {
	sigma = s;
	mu = m;

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
double DistNormal::pdf(RbDataType* variable) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	return RbStatistics::Normal::pdf(*mu,*sigma,x);
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
double DistNormal::lnPdf(RbDataType* variable) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	return RbStatistics::Normal::lnPdf(*mu,*sigma,x);
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
double DistNormal::cdf(RbDataType* variable) {
	// first some argument checking
	assert(typeid(*variable) == typeid(RbDouble));
	double x = ((RbDouble*)variable)->getValue();

	return RbStatistics::Normal::cdf(*mu,*sigma,x);
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
double DistNormal::quantile(RbDataType* variable) {
	RbDataType* first = arguments[0];
	assert(typeid(*first) == typeid(RbDouble));
	double p = ((RbDouble*)first)->getValue();
	return RbStatistics::Normal::quantile(mu,sigma,ma);
}


/*!
 * This function generates a normal(0,1) random variable.
 *
 * \brief Standard normal random variable.
 * \return Returns a standard normal random variable.
 * \throws Does not throw an error.
 */
double DistNormal::normalRv(void) {
	// call the random number generator

	return 0.0;
}


RbDataType DistNormal::getDataType() {
	//TODO implement this
	return RbDouble(0);
}
