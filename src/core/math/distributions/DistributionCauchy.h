/**
 * @file DistributionChauchy
 * This file contains the functions of the chauchy distribution.
 *
 * @brief Implementation of the chauchy distribution.
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

#include "RbPtr.h"

class RandomNumberGenerator;

namespace RbStatistics {
    
	namespace Cauchy {
        
        double                      pdf(double location, double scale, double x, bool give_log);          /*!< Cauchy(a,b) probability density */
        double                      pdf(double location, double scale, double x);                         /*!< Cauchy(a,b) probability density */
        double                      lnPdf(double location, double scale, double x);                       /*!< Cauchy(a,b) log_e probability density */
        double                      cdf(double a, double b, double x);                                    /*!< Cauchy(a,b) cumulative probability */
        double                      quantile(double a, double b, double p);                               /*!< Cauchy(a,b) quantile */
        double                      rv(double a, double b, RbPtr<RandomNumberGenerator> rng);             /*!< Cauchy(a,b) random variable */
	}
}