//
//  DistributionLKJ.cpp
//  revbayes
//
//  Created by Michael R. May on 2017-08-04.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#include <cmath>
#include "DistributionBeta.h"
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
    
//    // check that all rows are 1 and all colums are between -1 and 1
//    for(size_t r = 0; r < dim; ++r)
//    {
//        
//        if(z[r][r] != 1.0) {
//            return RbConstants::Double::neginf;
//        }
//        
//        for(size_t c = r; c < dim; ++c)
//        {
//            if(c > r)
//            {
//                if( z[r][c] > 1.0 | z[r][c] < -1.0 )
//                {
//                    return RbConstants::Double::neginf;
//                }
//            }
//        }
//    }
    
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
    
    MatrixReal P(dim); // this matrix holds the partial correlations
    MatrixReal S(dim); // this matrix holds the product-moment correlation matrix
    
    // S must start as an identity matrix
    for(size_t i = 0; i < dim; ++i)
    {
        S[i][i] = 1.0;
    }
    
    // this algorithm is only safe for eta > 1.
    // if eta < 1, we'll just return the identity matrix.
    // during MCMC, this only means that the initial value will not be from the prior
    if (eta < 1)
    {
        return S;
    }
    
    // initialize beta
    double beta = eta + (dim - 1) / 2;
    
    for(int k = 0; k < dim - 1; ++k)
    {
        
        // decrement beta
        beta -= 0.5;
        
        for(int i = k + 1; i < dim; ++i)
        {
            
            P[k][i] = RbStatistics::Beta::rv(beta, beta, rng) * 2.0 - 1.0; // sample the partial correlation
            
            double p = P[k][i]; // initial value for the product-moment correlation
//            if(k > 0) // recursively compute the product-moment correlation
//            {
                for(int l = k - 1; l >= 0; --l)
                {
                    p = p * pow( ( 1.0 - pow(P[l][i], 2) ) * ( 1.0 - pow(P[l][k], 2) ), 0.5) + P[l][i] * P[l][k];
                }
//            }
            
            S[i][k] = p;
            S[k][i] = p;
            
        }
        
    }
    
    return S;
    
}
