//
//  DistributionMultivariateNormal.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__DistributionMultivariateNormal__
#define __revbayes__DistributionMultivariateNormal__

#include <iostream>

#endif /* defined(__revbayes__DistributionMultivariateNormal__) */


#include <vector>
#include "PrecisionMatrix.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace MultivariateNormal {
            double                      pdf(const std::vector<double>& mu, const PrecisionMatrix& omega, const std::vector<double>& z);        /*!< MultivariateNormal(a[]) probability density */
            double                      lnPdf(const std::vector<double>& mu, const PrecisionMatrix& omega, const std::vector<double>& z);      /*!< MultivariateNormal(a[]) log_e probability density */
            std::vector<double>         rv(const std::vector<double>& mu, const PrecisionMatrix& omega, RandomNumberGenerator& rng);           /*!< MultivariateNormal(a[]) random variable */
            std::vector<double>         rvcov(const std::vector<double>& mu, const PrecisionMatrix& omega, RandomNumberGenerator& rng);           /*!< MultivariateNormal(a[]) random variable */
        }
    }
}

