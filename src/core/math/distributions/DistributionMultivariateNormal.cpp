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
 * \param sigma is a reference to a precision matrix containing the covariance
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::MultivariateNormal::pdfCovariance(const std::vector<double>& mu, const MatrixRealSymmetric& sigma, const std::vector<double> &z) {
	
    return exp(lnPdfCovariance(mu,sigma,z));
}


/*!
 * This function calculates the natural log of the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief Natural log of MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega0 is a reference to a precision matrix containing the covariance
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::MultivariateNormal::lnPdfCovariance(const std::vector<double>& mu, const MatrixRealSymmetric& sigma, const std::vector<double> &z) {
    
//    std::cerr << sigma << std::endl;
    
    sigma.update();
    
    const MatrixReal& tmp = sigma.getInverse();
    MatrixRealSymmetric omega = MatrixRealSymmetric(tmp);
  
    size_t dim = z.size();
    
    double s2 = 0;
    for (size_t i=0; i<dim; i++)   {
        double tmp = 0;
        for (size_t j=0; j<dim; j++)   {
            tmp += omega[i][j] * (z[j] - mu[j]);
        }
        s2 += (z[i] - mu[i]) * tmp;
    }
    
    double lnProb2 = - 0.5 * log(sigma.getLogDet()) - 0.5 * s2;
    double lnProb = 0.5 * log(omega.getLogDet()) - 0.5 * s2;
    
    return lnProb;
}    

/*!
 * This function generates a MultivariateNormal-distributed random variable.
 *
 * \brief MultivariateNormal random variable.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param sigma is a reference to a precision matrix containing the covariance
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the MultivariateNormal random variable.
 * \throws Does not throw an error.
 */

std::vector<double> RbStatistics::MultivariateNormal::rvCovariance(const std::vector<double>& mu, const MatrixRealSymmetric& sigma, RandomNumberGenerator& rng) {
        
    // actual work done inside MatrixRealSymmetric class
    // (more convenient: private members of sigma, e.g. eigenvalues and eigenvectors, are used here)
    sigma.update();
    size_t dim = sigma.getDim();
    std::vector<double> z(dim);
    sigma.drawNormalSampleCovariance(z);
    
    return z;
}

/*!
 * This function calculates the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega is a reference to a precision matrix containing the precision
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::MultivariateNormal::pdfPrecision(const std::vector<double>& mu, const MatrixRealSymmetric& omega, const std::vector<double> &z) {
	
    return exp(lnPdfPrecision(mu,omega,z));
}


/*!
 * This function calculates the natural log of the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief Natural log of MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega is a reference to a precision matrix containing the precision
 * \param z is a reference to a vector of doubles containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::MultivariateNormal::lnPdfPrecision(const std::vector<double>& mu, const MatrixRealSymmetric& omega, const std::vector<double> &z) {
    
    omega.update();
    
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
 * \param omega is a reference to a precision matrix containing the precision
* \param rng is a pointer to a random number object.
 * \return Returns a vector containing the MultivariateNormal random variable.
 * \throws Does not throw an error.
 */
std::vector<double> RbStatistics::MultivariateNormal::rvPrecision(const std::vector<double>& mu, const MatrixRealSymmetric& omega, RandomNumberGenerator& rng) {
    
    // actual work done inside MatrixRealSymmetric class
    // (more convenient: private members of omega, e.g. eigenvalues and eigenvectors, are used here)

    omega.update();
    size_t dim = omega.getDim();
    std::vector<double> z(dim);
    omega.drawNormalSamplePrecision(z);
    
    return z;
}
