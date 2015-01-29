//
//  DistributionMultivariateNormal.cpp
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//


#include "DistributionMultivariateNormal.h"
#include "DistributionNormal.h"
#include "EigenSystem.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathFunctions.h"
#include "RbMathLogic.h"
#include "RbStatisticsHelper.h"

#include <cmath>

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
double RbStatistics::MultivariateNormal::pdfCovariance(const std::vector<double>& mu, const MatrixReal& sigma, const std::vector<double> &x, double scale)
{
	
    return exp(lnPdfCovariance(mu,sigma,x,scale));
}


/*!
 * This function calculates the natural log of the probability density
 * for a MultivariateNormal-distributed random variable.
 *
 * \brief Natural log of MultivariateNormal probability density.
 * \param mu is a reference to a vector of doubles containing the mean
 * \param omega0 is a reference to a precision matrix containing the covariance
 * \param x is a reference to a vector of doubles containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::MultivariateNormal::lnPdfCovariance(const std::vector<double>& mu, const MatrixReal& sigma, const std::vector<double> &x, double scale)
{
    // we compute the precision matrix, which is the inverse of the covariance matrix
    // and then simply call the lnPDF for the precision matrix.
    // This simplifies the coding.
    MatrixReal omega = sigma.computeInverse();
    
    return lnPdfPrecision(mu, omega, x, scale);
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

std::vector<double> RbStatistics::MultivariateNormal::rvCovariance(const std::vector<double>& mu, const MatrixReal& sigma, RandomNumberGenerator& rng) {
        
    // actual work done inside MatrixReal class
    // (more convenient: private members of sigma, e.g. eigenvalues and eigenvectors, are used here)
//    sigma.update();
    
    size_t dim = sigma.getDim();
    const EigenSystem& eigensystem = sigma.getEigenSystem();
    std::vector<double> v(dim);
//    sigma.drawNormalSampleCovariance(z);
    
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(dim);
    for (size_t i=0; i<dim; i++)
    {
        w[i] = RbStatistics::Normal::rv(0, sqrt(eigen[i]), rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<dim; i++)
    {
        double tmp = 0;
        for (size_t j=0; j<dim; j++)
        {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }

    
    return v;
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
double RbStatistics::MultivariateNormal::pdfPrecision(const std::vector<double>& mu, const MatrixReal& omega, const std::vector<double> &x, double scale)
{
	
    return exp(lnPdfPrecision(mu,omega,x,scale));
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
double RbStatistics::MultivariateNormal::lnPdfPrecision(const std::vector<double>& mu, const MatrixReal& omega, const std::vector<double> &x, double scale)
{
    double logNormalize = -0.5 * log( RbConstants::TwoPI );
    
    double logDet = omega.getLogDet();
    if ( !RbMath::isAComputableNumber(logDet) )
    {
        return logDet;
    }
    
    size_t dim = x.size();
    std::vector<double> tmp   = std::vector<double>(dim,0.0);
    
    double s2 = 0;
    for (size_t i=0; i<dim; i++)
    {
        double tmp = 0;
        for (size_t j=0; j<dim; j++)
        {
            tmp += omega[i][j] * (x[j] - mu[j]);
        }
        s2 += (x[i] - mu[i]) * tmp;
    }
    
    double lnProb = dim * logNormalize + 0.5 * (logDet - dim * log(scale) - s2 / scale);
    
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
std::vector<double> RbStatistics::MultivariateNormal::rvPrecision(const std::vector<double>& mu, const MatrixReal& omega, RandomNumberGenerator& rng) {
    
    // actual work done inside MatrixReal class
    // (more convenient: private members of omega, e.g. eigenvalues and eigenvectors, are used here)

//    omega.update();
    size_t dim = omega.getDim();
    std::vector<double> v(dim);
    
    const EigenSystem &eigensystem = omega.getEigenSystem();
    
    // get the eigenvalues of the *precision* matrix
    const std::vector<double>& eigen = eigensystem.getRealEigenvalues();
    
    // draw the normal variate in eigen basis
    std::vector<double> w(dim);
    for (size_t i=0; i<dim; i++)
    {
        w[i] = RbStatistics::Normal::rv(0, 1.0 / sqrt(eigen[i]), rng);
    }
    
    // get the eigenvector
    const MatrixReal& eigenvect = eigensystem.getEigenvectors();
    
    // change basis
    for (size_t i=0; i<dim; i++)
    {
        double tmp = 0;
        for (size_t j=0; j<dim; j++)
        {
            tmp += eigenvect[i][j] * w[j];
        }
        v[i] = tmp;
    }

    
    return v;
}
