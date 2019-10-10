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

#include "DistanceMatrix.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace ExponentialError {
            
            double                      pdf(const DistanceMatrix& avgDistMat, double lambda, const DistanceMatrix& z);          /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(const DistanceMatrix& avgDistMat, double lambda, const DistanceMatrix& z);        /*!< Dirichlet(a[]) log_e probability density */
            DistanceMatrix              rv(const DistanceMatrix& avgDistMat, double lambda, RandomNumberGenerator& rng);        /*!< Dirichlet(a[]) random variable */

        }
    }
}


#endif    /* DISTRIBUTIONEXPONENTIALERROR_H */
