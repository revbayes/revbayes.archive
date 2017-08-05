//
//  DistributionLKJ.cpp
//  revbayes
//
//  Created by Michael R. May on 2017-08-04.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include <cmath>
#include "DistributionMultivariateNormal.h"
#include "DistributionLKJ.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbException.h"
#include "RbConstants.h"
#include "RbMathFunctions.h"
#include "RbStatisticsHelper.h"
#include "DistributionNormal.h"


using namespace RevBayesCore;

/*!
 * This function calculates the probability density
 * for a LKJ-distributed random variable.
 *
 * \brief LKJ probability density.
 * \param eta is the parameter of the LKJ distribution
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the probability density.
 * \throws Throws an RbException::ERROR.
 */
double RbStatistics::LKJ::pdf(double eta, const MatrixReal &z) {
	
    return exp(lnPdf(eta, z));
}



/*!
 * This function calculates the natural log of the probability density
 * for a LKJ-distributed random variable.
 *
 * \brief Natural log of LKJ probability density.
 * \param eta is the parameter of the LKJ distribution
 * \param z is a reference to a matrix containing the random variables.
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::LKJ::lnPdf(double eta, const MatrixReal &z)
{

    size_t dim = z.getNumberOfRows();
    
    // check that all rows are 1 and all colums are between -1 and 1
    for(size_t r = 0; r < dim; ++r)
    {
        
        if(z[r][r] != 1.0) {
            return RbConstants::Double::neginf;
        }
        
        for(size_t c = r; c < dim; ++c)
        {
            if(c > r)
            {
                if( z[r][c] > 1.0 | z[r][c] < -1.0 )
                {
                    return RbConstants::Double::neginf;
                }
            }
        }
    }
    
    z.setCholesky(true);
    
    return (eta - 1) * z.getLogDet();

}



/*!
 * This function generates a LKJ-distributed random variable.
 *
 * \brief LKJ random variable.
 * \param eta is the parameter of the LKJ distribution
 * \param rng is a pointer to a random number object.
 * \return Returns a vector containing the LKJ random variable.
 * \throws Does not throw an error.
 */
MatrixReal RbStatistics::LKJ::rv(double eta, size_t dim, RandomNumberGenerator& rng)
{

    // TODO: let's make this actually stochastic
    
    MatrixReal z(dim);
    for (size_t i = 0; i < dim; ++i) {
        z[i][i] = 1.0;
    }

    return z;
}



