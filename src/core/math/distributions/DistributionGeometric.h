/**
 * @file DistributionGeometric
 * This file contains the functions of the geometric distribution.
 *
 * @brief Implementation of the geometric distribution.
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
    
	namespace Geometric {
        
        double                      pdf(int n, double p);                                    /*!< Geometric(a,b) probability density */
        double                      pdf(int n, double p, bool asLog);                        /*!< Geometric(a,b) probability density */
        double                      lnPdf(int n, double p);                                  /*!< Geometric(a,b) log_e probability density */
        double                      cdf(int n, double p);                                    /*!< Geometric(a,b) cumulative probability */
        double                      quantile(double q, double p);                            /*!< Geometric(a,b) quantile */
        double                      rv(double a, double b, RbPtr<RandomNumberGenerator> rng);/*!< Geometric(a,b) random variable */
	}
}