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
            double                      pdf(const PrecisionMatrix& omega0, int df, const PrecisionMatrix& z);        /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(const PrecisionMatrix& omega0, int df, const PrecisionMatrix& z);      /*!< Dirichlet(a[]) log_e probability density */
            PrecisionMatrix         rv(const PrecisionMatrix& omega0, int df, RandomNumberGenerator& rng);           /*!< Dirichlet(a[]) random variable */
        }
    }
}


#endif /* defined(__revbayes__DistributionWishart__) */
