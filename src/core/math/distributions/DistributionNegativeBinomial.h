/**
 * @file DistributionNegativeBinomial
 * This file contains the functions of the Negative Binomial distribution.
 *
 * @brief Implementation of the Negative Binomial distribution.
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


#ifndef DistributionNegativeBinomial_H
#define DistributionNegativeBinomial_H

namespace RevBayesCore {

    class RandomNumberGenerator;

    namespace RbStatistics {
    
        namespace NegativeBinomial {
        
            double                      pdf(double n, double p, double x);                                    /*!< NegativeBinomial(n,p) probability density */
            double                      pdf(double n, double p, double q, double x, bool log);                /*!< NegativeBinomial(n,p) probability density */
            double                      lnPdf(double n, double p, double x);                                  /*!< NegativeBinomial(n,p) log_e probability density */
            double                      cdf(double n, double p, double x);                                    /*!< NegativeBinomial(n,p) cumulative probability */
            double                      quantile(double n, double p, double q);                               /*!< NegativeBinomial(n,p) quantile */
            int                         rv(double n, double p, RandomNumberGenerator& rng);             /*!< NegativeBinomial(n,p) random variable */
	        }
    }
}

#endif
