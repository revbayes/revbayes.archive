/*
 * File:   DistributionExponentialError.h
 * Author: David Cerny
 *
 * Created on October 8, 2019
 */

#ifndef DISTRIBUTIONEXPONENTIALERROR_H
#define DISTRIBUTIONEXPONENTIALERROR_H


#include <stddef.h>
#include <vector>
#include <iostream>

#include "MatrixReal.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace ExponentialError {
            
            double                      pdf(const MatrixReal& avgDistMat, double lambda, const MatrixReal& z);            /* !< Dirichlet(a[]) probability density */
            double                      lnPdf(const MatrixReal& avgDistMat, double lambda, const MatrixReal& z);            /* !< Dirichlet(a[]) log_e probability density */
            MatrixReal                  rv(const MatrixReal& avgDistMat, double lambda, RandomNumberGenerator& rng);     /* !< Dirichlet(a[]) random variable */

        }
    }
}


#endif    /* DISTRIBUTIONEXPONENTIALERROR_H */
