//
//  DistributionMultivariateNormal.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include "DistributionMultivariateNormal.h"


#include <cmath>
#include "DistributionMultivariateNormal.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbStatisticsHelper.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega0 is a reference to a precision matrix containing the precision
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::MultivariateNormal::pdf(const std::vector<double>& mu, const PrecisionMatrix& omega, const std::vector<double> &z) {
	
    return exp(lnPdf(mu,omega,z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief Natural log of MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega0 is a reference to a precision matrix containing the precision
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::MultivariateNormal::lnPdf(const std::vector<double>& mu, const PrecisionMatrix& omega, const std::vector<double> &z) {
    
    size_t dim = z.size();
    
    double s2 = 0;
    for (size_t i=0; i<dim; i++)   {
        double tmp = 0;
        for (size_t j=0; j<dim; j++)   {
            tmp += omega[i][j] * (z[j] - mu[j]);
        }
        s2 += (z[i] - mu[i]) * tmp;
    }
    
    double lnProb = 0.5 * log(omega.getLogDet()) - 0.5 * s2;
    
    return lnProb;
}



/*!
 * This function generates a MultivariateNormal-distributed random variable.
 *
 * \brief MultivariateNormal random variable.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega0 is a reference to a precision matrix containing the precision
* \param rng is a pointer to a random number object.
 * \return Returns a vector containing the MultivariateNormal random variable.
 * \throws Does not throw an error.
 */
std::vector<double> RbStatistics::MultivariateNormal::rv(const std::vector<double>& mu, const PrecisionMatrix& omega, RandomNumberGenerator& rng) {
    
    size_t dim = omega.getDim();
    std::vector<double> z(dim);
    omega.drawNormalSample(z);
    
    return z;
}

std::vector<double> RbStatistics::MultivariateNormal::rvcov(const std::vector<double>& mu, const PrecisionMatrix& omega, RandomNumberGenerator& rng) {
    
    size_t dim = omega.getDim();
    std::vector<double> z(dim);
    omega.drawNormalSampleFromInverse(z);
    
    return z;
}


