/**
 * @file DistributionChauchy
 * This file contains the functions of the chauchy distribution.
 *
 * @brief Implementation of the half-cauchy distribution (C+).
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author Andrew Magee
 * @license GPL version 3
 * @version 1.0
 * @since 2011-03-17, version 1.0
 *
 * $Id$
 */


#ifndef DistributionFiellerHinkley_H
#define DistributionFiellerHinkley_H

namespace RevBayesCore {
    
    class RandomNumberGenerator;

    namespace RbStatistics {
    
        namespace FiellerHinkley {
            double                      pdf(double mu1, double sd1, double mu2, double sd2, double rho, double x);
            double                      lnPdf(double mu1, double sd1, double mu2, double sd2, double rho, double x);
            double                      cdf(double mu1, double sd1, double mu2, double sd2, double rho, double x);
            double                      quantile(double mu1, double sd1, double mu2, double sd2, double rho, double x);
            double                      rv(double mu1, double sd1, double mu2, double sd2, double rho, RandomNumberGenerator& rng);
	
        }
    }
}

#endif
