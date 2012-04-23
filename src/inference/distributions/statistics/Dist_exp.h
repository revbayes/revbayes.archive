/**
 * @file
 * This file contains the declaration of Dist_exp, which is used to hold
 * parameters and functions related to an exponential distribution.
 *
 * @brief Declaration of Dist_exp
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

#ifndef Dist_exp_H
#define Dist_exp_H

#include "InferenceDistributionContinuous.h"
#include "RbValue.h"

#include <ostream>
#include <string>


class Dist_exp: public InferenceDistributionContinuous {

    public:
                                    Dist_exp(void);                                                     //!< Parser constructor

        // Basic utility functions
        Dist_exp*                   clone(void) const;                                                  //!< Clone object

        // distribution functions
        double                      cdf(double q);                                                      //!< Cumulative density
        double                      quantile(double p);                                                 //!< Quantile

    private:
        double                      lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
        double                      pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
        void                        rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
        void                        setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    
        // parameters
        RbValue<double*>            lambda;
    

};

#endif

