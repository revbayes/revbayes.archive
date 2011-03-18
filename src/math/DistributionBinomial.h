/**
 * @file DistributionBinomial
 * This file contains the functions of the binomial distribution.
 *
 * @brief Implementation of the binomial distribution.
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

class RandomNumberGenerator;

namespace RbStatistics 
{
    
    
	namespace Binomial {
        
        double                      pdf(double a, double b, double x);                                    /*!< Binomial(a,b) probability density */
        double                      lnPdf(double a, double b, double x);                                  /*!< Binomial(a,b) log_e probability density */
        double                      cdf(double a, double b, double x);                                    /*!< Binomial(a,b) cumulative probability */
        double                      quantile(double a, double b, double p);                               /*!< Binomial(a,b) quantile */
        double                      rv(double a, double b, RandomNumberGenerator* rng);                   /*!< Binomial(a,b) random variable */
	}
    
}