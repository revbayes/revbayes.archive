/*
 * DistNormal.cpp
 *
 *  Created on: 25 aug 2009
 *      Author: Sebastian
 */

#include <cmath>

#include "DistNormal.h"
#include "../../datatypes/RbDataType.h"
#include "../../datatypes/primary/RbDouble.h"
#include "../../utils/RbMath.h"
#include "../../utils/RbStatistics.h"

DistNormal::DistNormal(double* s, double* m) {
	sigma = s;
	mu = m;
}

DistNormal::DistNormal(DistNormal& d) {
	sigma = d.sigma;
	mu = d.mu;
}

DistNormal::~DistNormal() {
	// TODO Auto-generated destructor stub
}

/**
 * @brief Copy this object
 *
 * This is a call of the copy constructor used from the base class
 *
 * @return     return a deep copy of the object
 *
 */
RbObject* DistNormal::clone(void) {
	return new DistNormal(*this);
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

	double pdf = RbStatistics::Normal::pdf(*mu,*sigma,x);

	return pdf;
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
	assert(typeid(*variable) == typeid(RbDouble));
	double p = ((RbDouble*)variable)->getValue();
	return RbStatistics::Normal::quantile(*mu,*sigma,p);
}


/*!
 * This function generates a normal random variable.
 *
 * \brief Standard normal random variable.
 * \return Returns a standard normal random variable.
 * \throws Does not throw an error.
 */
RbDataType* DistNormal::rv(void) {
	// call the random number generator

	return NULL;
}


RbDataType* DistNormal::getDataType() {
	//TODO implement this
	return new RbDouble(0);
}
