/**
 * @file DistributionMultinomial
 * This file contains the functions of the multinomial distribution.
 *
 * @brief Implementation of the multinomial distribution.
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
#include <vector>

class RandomNumberGenerator;

namespace RbStatistics {
    
	namespace Multinomial {
        double                      pdf(const std::vector<double>& p, const std::vector<unsigned int>& x);  /*!< Multinomial(p[]) probability */
        double                      pdf(const std::vector<double>& p, const std::vector<int>& x);           /*!< Multinomial(p[]) probability */
        double                      pdf(const std::vector<double>& p, const std::vector<double>& x);        /*!< Multinomial(p[]) probability */
        double                      lnPdf(const std::vector<double>& p, const std::vector<unsigned int>& x);/*!< Multinomial(p[]) log_e probability */
        double                      lnPdf(const std::vector<double>& p, const std::vector<int>& x);         /*!< Multinomial(p[]) log_e probability */
        double                      lnPdf(const std::vector<double>& p, const std::vector<double>& x);      /*!< Multinomial(p[]) log_e probability */
        std::vector<int>            rv(const std::vector<double>& p, RbPtr<RandomNumberGenerator> rng);     /*!< Multinomial(p[]) random variable */
        std::vector<int>            rv(const std::vector<double>& p, int n, RbPtr<RandomNumberGenerator> rng);/*!< Multinomial(p[]) random variable */
	}
}