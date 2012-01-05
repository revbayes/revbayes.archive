/**
 * @file DistributionChisq
 * This file contains the functions of the chi-square distribution.
 *
 * @brief Implementation of the chi-square distribution.
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
#include "RandomNumberGenerator.h"
#include "RbMathFunctions.h"

/*!
 * This function calculates the probability density 
 * for a chi-square distributed random variable.
 *
 * \brief Chi-square probability density.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::pdf(double v, double x) {
    
	double pdf;
	if ( x < 0.0 )
        {
		pdf = 0.0;
        }
	else
        {
		double b = v / 2.0;
		pdf = exp( -0.5 * x ) * pow( x, ( b - 1.0 ) ) / ( pow( 2.0, b ) * RbMath::gamma( b ) );
        }
	return pdf;
}

/*!
 * This function calculates the natural log of the probability density 
 * for a chi-square distributed random variable.
 *
 * \brief Natural log of chi-square probability density.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the natural log of the probability density.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::lnPdf(double v, double x) {
    
	double b = v / 2.0;
	return ( -(b * log(2.0) + RbMath::lnGamma(b)) - b + (b - 1.0) * std::log(x) );
}

/*!
 * This function calculates the cumulative probability  
 * for a chi-square distributed random variable.
 *
 * \brief Chi-square cumulative probability.
 * \param v is the degrees of freedom parameter of the chi-square. 
 * \param x is the chi-square random variable. 
 * \return Returns the cumulative probability.
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::cdf(double v, double x) {
    
	return RbStatistics::Gamma::cdf( v / 2.0, 0.5, x );
}

/*!
 * This function calculates the quantile of a chi square distribution with v
 * degrees of freedom.
 *
 * \brief Quantile of a chi square distribution.
 * \param v is the degrees of freedom of the chi square. 
 * \param prob is the probability up to the quantile. 
 * \return Returns quantile value (or -1 if in error). 
 * \throws Does not throw an error.
 */
double RbStatistics::ChiSquare::quantile(double v, double prob) {

    return RbStatistics::Gamma::quantile(0.5 * v, 2.0, prob);    
}

double RbStatistics::ChiSquare::rv(double v, RbPtr<RandomNumberGenerator> rng) {
    
	/* Cast the degrees of freedom parameter as an integer. We will see
     if there is a decimal remainder later. */
	int n = (int)(v);
	
	double x2;
	if ( (double)(n) == v && n <= 100 )
        {
		/* If the degrees of freedom is an integer and less than 100, we
         generate our chi-square random variable by generating v
         standard normal random variables, squaring each, and taking the
         sum of the squared random variables. */
		x2 = 0.0;
		for (int i=0; i<n; i++)
            {
			double x = RbStatistics::Normal::rv(0.0, 1.0, rng);
			x2 += x * x;
            }
        }
	else
        {
		/* Otherwise, we use the relationship of the chi-square to a gamma
         (it is a special case of the gamma) to generate the chi-square
         random variable. */
		x2 = RbStatistics::Gamma::rv(v/2.0, 0.5, rng);
        }
	return x2;
}
