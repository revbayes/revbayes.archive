/* 
 * File:   DistributionInverseWishart.h
 * Author: nl
 *
 * Created on 15 juillet 2014, 10:13
 */

#ifndef DISTRIBUTIONINVERSEWISHART_H
#define	DISTRIBUTIONINVERSEWISHART_H


#include <iostream>
#include "PrecisionMatrix.h"

namespace RevBayesCore {
    
    class RandomNumberGenerator;
    
    
    namespace RbStatistics {
        
        namespace InverseWishart {
            
            // standard form:
            // sigma0: scale matrix
            // df: degrees of freedom
            // by definition: Z = \sum_i X_i X_i', where X_i ~ Normal(0, covariance = sigma0)
            double                      pdf(const PrecisionMatrix& sigma0, size_t df, const PrecisionMatrix& z);        /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(const PrecisionMatrix& sigma0, size_t df, const PrecisionMatrix& z);      /*!< Dirichlet(a[]) log_e probability density */
            PrecisionMatrix             rv(const PrecisionMatrix& sigma0, size_t df, RandomNumberGenerator& rng);           /*!< Dirichlet(a[]) random variable */

            // InverseWishart of parameters sigma0 = kappa * I and df degrees of freedom
            double                      pdf(double kappa, size_t df, const PrecisionMatrix& z);        /*!< Dirichlet(a[]) probability density */
            double                      lnPdf(double kappa, size_t df, const PrecisionMatrix& z);      /*!< Dirichlet(a[]) log_e probability density */
            PrecisionMatrix             rv(double kappa, size_t dim, size_t df, RandomNumberGenerator& rng);           /*!< Dirichlet(a[]) random variable */

        }
    }
}


#endif	/* DISTRIBUTIONINVERSEWISHART_H */

