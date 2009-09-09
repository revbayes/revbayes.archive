/*
 *  DistExponential.cpp
 *  REvBayes
 *
 *  Created by John Huelsenbeck on 8/18/09.
 *  Copyright 2009 University of California, Berkeley. All rights reserved.
 *
 */

#include "DistExponential.h"
#include "../datatypes/RbDataType.h"
#include "../datatypes/primary/RbDouble.h"
#include "../utils/RbMath.h"

DistExponential::DistExponential() {
	// TODO Auto-generated constructor stub

}

DistExponential::~DistExponential() {
	// TODO Auto-generated destructor stub
}

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
double DistExponential::pdf(double lambda, double x) {

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
double DistExponential::lnPdf(double lambda, double x) {

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
double DistExponential::cdf(double lambda, double x) {

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
double DistExponential::quantile(double lambda, double p) {

	return -(1.0 / lambda) * std::log(1.0 - p);
}

