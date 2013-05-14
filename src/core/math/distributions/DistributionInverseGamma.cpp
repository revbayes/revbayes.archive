//
//  DistributionInverseGamma.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 3/19/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <cmath>

#include "DistributionChisq.h"
#include "DistributionInverseGamma.h"
#include "DistributionNormal.h"
#include "RbStatisticsHelper.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a InverseGamma-distributed random variable.
 *
 * \brief InverseGamma probability density.
 * \param the shape parameter of the InverseGamma.
 * \param the scale parameter of the InverseGamma.
 * \param x is the InverseGamma random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseGamma::pdf(double shape, double scale, double x) {
    
	return (pow(scale, shape) / RbMath::gamma(shape)) * pow(x, -(shape + 1.0)) * exp(-scale / x);
}


/*!
 * This function calculates the probability density
 * for a InverseGamma-distributed random variable.
 *
 * \brief InverseGamma probability density.
 * \param the shape parameter of the InverseGamma.
 * \param the scale parameter of the InverseGamma.
 * \param x is the InverseGamma random variable.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseGamma::pdf(double shape, double scale, double x, bool isLog) {
    
    //    double pr;
    //    if (shape < 0 || scale <= 0) {
    //        std::ostringstream s;
    //        s << "Cannot compute the pdf for the InverseGamma distribution for shape = " << shape << " and scale = " << scale;
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
 * for a InverseGamma-distributed random variable.
 *
 * \brief Natural log of InverseGamma probability density.
 * \param the shape parameter of the InverseGamma.
 * \param the scale parameter of the InverseGamma.
 * \param x is the InverseGamma random variable.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseGamma::lnPdf(double shape, double scale, double x) {
    
	return shape * log(scale) - RbMath::lnGamma(shape) + (-(shape + 1.0)) * log(x) - (scale/x);
}

/*!
 * This function calculates the cumulative probability
 * for a InverseGamma-distributed random variable.
 *
 * \brief InverseGamma cumulative probability.
 * \param the shape parameter of the InverseGamma.
 * \param the scale parameter of the InverseGamma.
 * \param x is the InverseGamma random variable.
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseGamma::cdf(double shape, double scale, double x) {

    double lowerIncompleteGamma = RbMath::incompleteGamma( scale/x, shape, RbMath::lnGamma(shape) );
    double gamma = RbMath::gamma(shape);
    
    return (gamma - lowerIncompleteGamma) / gamma;
}

/*!
 * This function returns the quantile of a InverseGamma probability
 * distribution.
 *
 * \brief InverseGamma quantile.
 * \param the shape parameter.inc
 * \param the scale parameter.
 * \param p is the probability up to the quantile.
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::InverseGamma::quantile(double shape, double scale, double p) {
    
    
    // NOT IMPLEMENTED...
    return 0.0;
    
	return RbStatistics::ChiSquare::quantile(p, 2.0 * shape) / (2.0 * scale);
    //	return RbStatistics::Helper::pointChi2(p, 2.0 * shape) / (2.0 * scale);
}


double RbStatistics::InverseGamma::rv(double shape, double scale, RandomNumberGenerator& rng) {
    
	return (RbStatistics::Helper::rndGamma(shape, rng) * scale);
}

