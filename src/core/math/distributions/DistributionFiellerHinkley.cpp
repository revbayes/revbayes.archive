/**
 * @file DistributionFiellerHinkley
 * This file contains the functions of the chauchy distribution.
 *
 * @brief Implementation of the chauchy distribution.
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

#include "DistributionFiellerHinkley.h"
#include "DistributionMultivariateNormal.h"
#include "DistributionNormal.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density 
 * for a Fieller-Hinkley-distributed random variable.
 *
 * \brief Fieller-Hinkley probability density, describes ratio of two Normal R.V.s, X and Y.
 * \param mu1 is the mean of the first Normal RV
 * \param sd1 is the sd of the first Normal RV
 * \param mu2 is the mean of the second Normal RV
 * \param sd2 is the sd of the second Normal RV
 * \param rho is the correlation between X and Y
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */

double RbStatistics::FiellerHinkley::pdf(double mu1, double sd1, double mu2, double sd2, double rho, double x)
{
    double one_minus_rho_sq = (1 - rho * rho);
    double var1 = sd1 * sd1;
    double var2 = sd2 * sd2;
    
    double aw = std::sqrt( (x * x)/(var1) - (2 * rho * x)/(sd1 * sd2) + 1/(var2) );
    
    double bw = (mu1 * x) / (var1) - (rho * (mu1 + mu2 * x))/(sd1 * sd2) + mu2/(var2);
    
    double c  = (mu1 * mu1) / (var1) - (2 * rho * mu1 * mu2)/(sd1 * sd2) + (mu2 * mu2)/(var2);
    
    double dw = std::exp((bw * bw - c * aw * aw)/(2 * (one_minus_rho_sq) * aw * aw));
    
    double inside = bw / (sqrt(one_minus_rho_sq * aw));
    
    double fw = (bw * dw / std::sqrt(RbConstants::TwoPI * sd1 * sd2 * aw * aw * aw));
    fw *= (RbStatistics::Normal::cdf(inside) - RbStatistics::Normal::cdf(-inside));
    fw += (sqrt(one_minus_rho_sq) / (RbConstants::PI * sd1 * sd2 * aw*aw) * std::exp(-c / one_minus_rho_sq));
    
    return ( fw );
}

double RbStatistics::FiellerHinkley::lnPdf(double mu1, double sd1, double mu2, double sd2, double rho, double x)
{
    double fw = FiellerHinkley::pdf(mu1, sd1, mu2, sd2, rho, x);
    return ( std::log(fw) );
}

double RbStatistics::FiellerHinkley::cdf(double mu1, double sd1, double mu2, double sd2, double rho, double x)
{
    throw RbException("CDF not currently implemented.");
    return ( RbConstants::Double::nan );
}

double RbStatistics::FiellerHinkley::quantile(double mu1, double sd1, double mu2, double sd2, double rho, double x)
{
    throw RbException("Quantiles not implemented.");
    return ( RbConstants::Double::nan );
}

double RbStatistics::FiellerHinkley::rv(double mu1, double sd1, double mu2, double sd2, double rho, RandomNumberGenerator& rng) {
    // make mu1 and mu2 into vector mu
    std::vector<double> mu = std::vector<double>(2, mu1);
    mu[1] = mu2;
    
    // make rho, sd1, and sd2 into covariance matrix sigma
    MatrixReal sigma(2, 2);
    sigma[0][0] = sd1 * sd1;
    sigma[0][1] = rho * sd1 * sd2;
    sigma[1][0] = rho * sd1 * sd2;
    sigma[1][1] = sd2 * sd2;
    
    // call MVN::rvCovariance(...,scale=1)
    std::vector<double> x = RbStatistics::MultivariateNormal::rvCovariance(mu, sigma, rng, 1);
	return ( x[0]/x[1] );
}