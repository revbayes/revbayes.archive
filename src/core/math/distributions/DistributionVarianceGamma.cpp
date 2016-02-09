//
//  DistributionVarianceGamma.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 2/8/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include <cmath>
#include <iostream>

#include "RbMathCombinatorialFunctions.h"
#include "DistributionVarianceGamma.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbException.h"
#include "RbStatisticsHelper.h"

#include <boost/math/special_functions/bessel.hpp>

using namespace RevBayesCore;

double RbStatistics::VarianceGamma::pdf(double mu, double kappa, double tau, double x) {
    
    double t = 1.0;
    double kappa4 = 4 * kappa;
    
    // first part, arithmetic
    double partOne = 0.0;
    partOne = pow(2, (2 * t - 3 * kappa) / kappa4);
    partOne *= pow(kappa, -(2 * t + kappa) / kappa4);
    partOne /= RbMath::gamma(t / kappa) * sqrt(RbConstants::PI * tau * tau);
    partOne *= pow(tau * tau / (x * x), (-2 * t + kappa) / kappa4);
    // TODO: might be faster in logspace
    
    // second part, Bessel function of the second kind
    
    // use:
    //template <class T1, class T2>
    //calculated-result-type cyl_neumann(T1 v, T2 x);
    
    double besselV = fabs(t / kappa - 1 / 2);
    double besselX = fabs(sqrt(2 * x * x / (kappa * tau * tau)));
    double partTwo = boost::math::cyl_neumann(besselV, besselX);
    
    return partOne * partTwo;
}

/*!
 * This function calculates the natural log of the probability for a
 * VarianceGamma distribution.
 *
 * \brief Natural log of VarianceGamma probability.
 * \param lambda is the rate parameter of the VarianceGamma.
 * \param x is the value of the random variable.
 * \return Returns the natural log of the probability.
 * \throws Does not throw an error.
 */
double RbStatistics::VarianceGamma::lnPdf(double mu, double kappa, double tau, double x) {
    
    return log(pdf(mu, kappa, tau, x));
    
}

/*!
 * This function calculates the cumulative probability for a
 * VarianceGamma distribution.
 *
 * \brief VarianceGamma cumulative probability.
 * \param lambda is the rate parameter of the VarianceGamma.
 * \param x is the value of the random variable.
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::VarianceGamma::cdf(double mu, double kappa, double tau, double x) {
    // cdf will be over pdf for sum(n), n << \inf
    return 0.0;
}

/*!
 * This function returns the quantile of a VarianceGamma probability
 * distribution.
 *
 * \brief VarianceGamma(lambda) quantile.
 * \param lambda is the rate parameter of the VarianceGamma.
 * \param p is the probability up to the quantile.
 * \return Returns the quantile.
 * \throws Does not throw an error.
 */
double RbStatistics::VarianceGamma::quantile(double mu, double kappa, double tau, double p) {
    
    return 0.0;
}

/*!
 * This function generates a VarianceGamma-distributed random
 * variable with parameter lambda.
 *
 * \brief VarianceGamma(lambda) random variable.
 * \param lambda the rate parameter of the VarianceGamma.
 * \param rng is a pointer to a random number object.
 * \return This function returns a VarianceGamma-distributed integer.
 * \throws Does not throw an error.
 */
double RbStatistics::VarianceGamma::rv(double mu, double kappa, double tau, RandomNumberGenerator& rng) {
    
    
    double x = 0.0;
    
    return x;
}