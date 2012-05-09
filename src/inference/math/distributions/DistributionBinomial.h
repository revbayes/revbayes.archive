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

namespace RbStatistics {
    
	namespace Binomial {
        
        double                      pdf(double n, double p, double x);                                    /*!< Binomial(n,p) probability density */
        double                      pdf(double n, double p, double q, double x, bool log);                /*!< Binomial(n,p) probability density */
        double                      lnPdf(double n, double p, double x);                                  /*!< Binomial(n,p) log_e probability density */
        double                      cdf(double a, double b, double x);                                    /*!< Binomial(n,p) cumulative probability */
        double                      quantile(double a, double b, double p);                               /*!< Binomial(n,p) quantile */
        double                      rv(double a, double b, RandomNumberGenerator& rng);             /*!< Binomial(n,p) random variable */
	}
}