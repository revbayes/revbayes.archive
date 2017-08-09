//
//  DistributionLKJ.h
//  revbayes
//
//  Created by Michael R. May on 2017-08-04.
//  Copyright (c) 2014 revbayes team. All rights reserved.
//

#ifndef DistributionLKJ_H
#define DistributionLKJ_H

#include <iostream>
#include "MatrixReal.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace LKJ {
            
            double                          pdf(double eta, const MatrixReal& z);                   /*!< Dirichlet(a[]) probability density */
            double                          lnPdf(double eta, const MatrixReal& z);                 /*!< Dirichlet(a[]) log_e probability density */
            MatrixReal                      rv(double eta, size_t dim, RandomNumberGenerator& rng); /*!< Dirichlet(a[]) random variable */

        }
    }
}

#endif /* defined(__revbayes__DistributionLKJ__) */
