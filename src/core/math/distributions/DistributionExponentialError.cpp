/*
 * File:   DistributionExponentialError.cpp
 * Author: David Cerny
 *
 * Created on October 8, 2019
 */


#include "DistributionExponentialError.h"

#include <cmath>

#include "DistributionDirichlet.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "Cloneable.h"
#include "RbVector.h"
#include "RbVectorImpl.h"
#include "DistanceMatrix.h"

using namespace RevBayesCore;

/*!
 * This function calculates the probability density for a matrix of
 * random variables whose distance from the average distance matrix
 * is exponentially distributed.
 *
 * \brief ExponentialError probability density.
 * \param avgDistMat is a reference to the average distance matrix.
 * \param lambda is the rate parameter of the exponential.
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */

double RbStatistics::ExponentialError::pdf(const DistanceMatrix &avgDistMat, double lambda, const DistanceMatrix &z)
{
    return exp(lnPdf(avgDistMat, lambda, z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a matrix of random variables whose distance from the average
 * distance matrix is exponentially distributed.
 *
 * \brief Natural log of ExponentialError probability density.
 * \param avgDistMat is a reference to the average distance matrix.
 * \param lambda is the rate parameter of the exponential.
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */

double RbStatistics::ExponentialError::lnPdf(const DistanceMatrix &avgDistMat, double lambda, const DistanceMatrix &z)
{
    
    if ( avgDistMat.getSize() != z.getSize() )
    {
        return RbConstants::Double::neginf;
    }
    
    double dist = 0;
    
    for (size_t i=0; i<avgDistMat.getSize(); i++)
    {
        for (size_t j=0; j<avgDistMat.getSize(); j++)
        {
            dist += pow(z[i][j] - avgDistMat[i][j], 2.0);
        }
    }
    
    return (std::log(lambda) - lambda * dist);
}



/*!
 * This function generates a matrix of random variables whose distance
 * from the average distance matrix is exponentially distributed.
 *
 * \brief ExponentialError random variable.
 * \param avgDistMat is a reference to the average distance matrix.
 * \param lambda is the rate parameter of the exponential.
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the InverseWishart random variable.
 * \throws Does not throw an error.
 */

DistanceMatrix RbStatistics::ExponentialError::rv(const DistanceMatrix &avgDistMat, double lambda, RandomNumberGenerator& rng)
{
 
    size_t dim = avgDistMat.getSize();
    
    // first, draw from the exponential of rate lambda:
    double u = rng.uniform01();
    double draw = -(1.0/lambda) * std::log(u);
    
    // next, get a vector of draws from a Dirichlet distribution that has half as many
    // elements as there are off-diagonal elements in the average distance matrix:
    size_t vect_len = (dim * (dim - 1))/2;
    std::vector<double> DirichParams(vect_len, 1);
    std::vector<double> DirichRandomVars = RbStatistics::Dirichlet::rv(DirichParams, rng);
    
    // get an equally long vector of signs (-1 or 1):
    std::vector<double> Signs(vect_len);
    for (size_t i=0; i<vect_len; i++)
    {
        double signProb = rng.uniform01();
        if (signProb <= 0.5)
        {
            Signs[i] = -1;
        } else {
            Signs[i] = 1;
        }
    }
    
    // initialize the z matrix from avgDistMat using the copy constructor:
    DistanceMatrix z = DistanceMatrix(avgDistMat);
    
    // fill it in:
    for (size_t i=0; i<dim; i++)
    {
        for (size_t j=0; j<dim; j++)
        {
            if (i > j) // lower triangular
            {
                size_t index = (j * (2 * dim - j - 1))/2 + (i - j - 1);
                z[i][j] += sqrt(draw * DirichRandomVars[index]) * Signs[index];
            }
            else if (j > i) // upper triangular
            {
                size_t index = (i * (2 * dim - i - 1))/2 + (j - i - 1);
                z[i][j] += sqrt(draw * DirichRandomVars[index]) * Signs[index];
            }
        }
    }
 
    return z;
}
