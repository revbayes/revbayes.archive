/**
 * @file
 * This file contains the declaration of Dist_beta, which is used to hold
 * parameters and functions related to an beta distribution.
 *
 * @brief Declaration of Dist_beta
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_beta_H
#define Dist_beta_H

#include "DistributionContinuous.h"

#include <ostream>
#include <string>


class Dist_beta: public DistributionContinuous {

public:
    Dist_beta(void);                                                                                //!< constructor
    
    // Basic utility functions
    Dist_beta*                  clone(void) const;                                                  //!< Clone object
    
    // distribution functions
    double                      cdf(double q);                                                      //!< Cumulative density
    double                      quantile(double p);                                                 //!< Quantile
    
private:
    double                      lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
    double                      pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
    void                        rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
    void                        setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    
    // parameters
    RbValue<double*>            alpha;
    RbValue<double*>            beta;
    
};

#endif

