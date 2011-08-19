/**
 * @file DistributionLognormal
 * This file contains the functions of the normal distribution.
 *
 * @brief Implementation of the normal distribution.
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


#include "DistributionLognormal.h"
#include "DistributionNormal.h"
#include "RbConstants.h"
#include "RandomNumberGenerator.h"
#include "RbStatisticsHelper.h"


/*!
 * This function calculates the probability density 
 * for a standard normally-distributed random variable.
 *
 * \brief Standard normal probability density.
 * \param x is the normal random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Lognormal::pdf(double x) {
    
	//return exp( -0.5 * x * x )  / RbConstants::SQRT_2PI;
	
	double y = (log(x) - 0.0) / 1.0;
    return (RbConstants::ONE_SQRT_2PI * exp(-0.5 * y * y)  /	 (x * 1.0));
	//return RbStatistics::Normal::pdf(
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
double RbStatistics::Lognormal::pdf(double m, double s, double x) {
    
//	double y = ( x - mu ) / sigma;
//	exp( -0.5 * y * y )  / ( sigma * sqrt ( 2.0 * RbConstants::PI ) );
//	return RbStatistics::Normal::pdf(
	double y = (log(x) - m) / s;
    return (RbConstants::ONE_SQRT_2PI * exp(-0.5 * y * y)  /	 (x * s));
}

/*!
 * This function calculates the natural log of the probability density 
 * for a standard normally-distributed random variable.
 *
 * \brief Natural log of standard normal probability density.
 * \param x is the normal random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::Lognormal::lnPdf(double x) {
    
	double y = (log(x) - 0.0) / 1.0;
	return -(RbConstants::LN_SQRT_2PI   + 0.5 * y * y + log(x * 1.0));
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
double RbStatistics::Lognormal::lnPdf(double m, double s, double x) {
    
	double y = (log(x) - m) / s;
	return -(RbConstants::LN_SQRT_2PI   + 0.5 * y * y + log(x * s));
}

/*!
 * This function calculates the cumulative probability 
 * for a standard normally-distributed random variable.
 *
 * \brief Standard normal cumulative probability.
 * \param x is the normal random variable. 
 * \return Returns the cumulative probability.
 * \see Adams, A. G. 1969. Areas under the normal curve. Cojputer J. 12:197-198.
 * \throws Does not throw an error.
 */
double RbStatistics::Lognormal::cdf(double x) {
    
	//double cdf;
	//double q;
    
	//return cdf;
	
	return RbStatistics::Normal::cdf(log(x));
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
double RbStatistics::Lognormal::cdf(double m, double s, double x) {
    
	return RbStatistics::Normal::cdf(m, s, log(x));
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
double RbStatistics::Lognormal::quantile(double p) {
    
	return exp(RbStatistics::Normal::quantile(p));
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
double RbStatistics::Lognormal::quantile(double m, double s, double p) {
	
//	double z = Lognormal::quantile(p);
//	double x = z * sigma + mu;
//	return x;
	return exp(RbStatistics::Normal::quantile(m, s, p));
}

double RbStatistics::Lognormal::rv(RandomNumberGenerator* rng) {
    
//	double v1 = 0.0;
//	double v2 = 0.0; // NOTE: We should eventually implement this so you generate and
//	//       return the extra normal random variable that is generated
//	double rsq = 0.0;
//	do
//	{
//		v1 = 2.0 * rng->uniform01() - 1.0;
//		v2 = 2.0 * rng->uniform01() - 1.0;
//		rsq = v1 * v1 + v2 * v2;
//	} while ( rsq >= 1.0 || rsq == 0.0 );
//	double fac = sqrt(-2.0 * log(rsq)/rsq);
//	//extraNormalRv = v1 * fac;
//	//availableNormalRv = true;
//	return ( v2 * fac );
	
	return exp(RbStatistics::Normal::rv(rng));
	
}

double RbStatistics::Lognormal::rv(double m, double s, RandomNumberGenerator* rng) {
    
//	double v1 = 0.0;
//	double v2 = 0.0; // NOTE: We should eventually implement this so you generate and
//    // return the extra normal random variable that is generated
//	double rsq = 0.0;
//	do
//	{
//		v1 = 2.0 * rng->uniform01() - 1.0;
//		v2 = 2.0 * rng->uniform01() - 1.0;
//		rsq = v1 * v1 + v2 * v2;
//	} while ( rsq >= 1.0 || rsq == 0.0 );
//	double fac = sqrt(-2.0 * log(rsq)/rsq);
//	//extraNormalRv = v1 * fac;
//	//availableNormalRv = true;
//	return ( mu + sigma * (v2 * fac) );
	
	return exp(RbStatistics::Normal::rv(m, s, rng));
}
