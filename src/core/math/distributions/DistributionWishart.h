//
//  DistributionWishart.h
//  revbayes
//
//  Created by Nicolas Lartillot on 2014-03-28.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef __revbayes__DistributionWishart__
#define __revbayes__DistributionWishart__

#include <iostream>


#include "PrecisionMatrix.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace Wishart {
            
            // standard form:
            // omega0: scale matrix
            // df: degrees of freedom
            // by definition: Z = \sum_i X_i X_i', where X_i ~ Normal(0, covariance = omega0)
            double                      pdf(const PrecisionMatrix& omega0, size_t df, const PrecisionMatrix& z);        /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(const PrecisionMatrix& omega0, size_t df, const PrecisionMatrix& z);      /*!< Dirichlet(a[]) log_e probability density */
            PrecisionMatrix             rv(const PrecisionMatrix& omega0, size_t df, RandomNumberGenerator& rng);           /*!< Dirichlet(a[]) random variable */

            // Wishart of parameters omega0 = kappa * I and df degrees of freedom
            double                      pdf(double kappa, size_t df, const PrecisionMatrix& z);        /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(double kappa, size_t df, const PrecisionMatrix& z);      /*!< Dirichlet(a[]) log_e probability density */
            PrecisionMatrix             rv(double kappa, size_t dim, size_t df, RandomNumberGenerator& rng);           /*!< Dirichlet(a[]) random variable */

        }
    }
}


#endif /* defined(__revbayes__DistributionWishart__) */
