/**
 * @file DistributionGamma
 * This file contains the functions of the gamma distribution.
 *
 * @brief Implementation of the gamma distribution.
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

#include "DistributionChisq.h"
#include "DistributionGamma.h"
#include "DistributionNormal.h"
//#include "DistributionPoisson.h"
#include "RbStatisticsHelper.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

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
double RbStatistics::Gamma::pdf(double shape, double scale, double x, bool isLog) {
//    double pr;
//    if (shape < 0 || scale <= 0) {
//        std::ostringstream s;
//        s << "Cannot compute the pdf for the gamma distribution for shape = " << shape << " and scale = " << scale;
//        throw (RbException(s));
//	    }
// if (x < 0)
//	        return 0.0;
// 
// if (shape == 0) /* point mass at 0 */
//	        return (x == 0)? RbConstants::Double::inf : 0.0;
//
// if (x == 0) {
//	        if (shape < 1) return RbConstants::Double::inf;
//	        if (shape > 1) return 0.0;
//	        /* else */
//	        return isLog ? -log(scale) : 1 / scale;
//	    }
//
// if (shape < 1) {
//	        pr = RbStatistics::Poisson::pdf(shape, x/scale, isLog);
//	        return isLog ?  pr + log(shape/x) : pr*shape/x;
//	    }
// /* else  shape >= 1 */
// pr = RbStatistics::Poisson::pdf(shape-1, x/scale, isLog);
// return isLog ? pr - log(scale) : pr/scale;
    
    return isLog ? pdf(shape, scale, exp(x)) : pdf(shape, scale, x);
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
    
	return RbStatistics::ChiSquare::quantile(p, 2.0 * a) / (2.0*b);
}

double RbStatistics::Gamma::rv(double a, double b, RandomNumberGenerator* rng) {
    
	return (RbStatistics::Helper::rndGamma(a, *rng) / b);
}

