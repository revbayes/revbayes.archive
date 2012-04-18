/**
 * @file
 * This file contains the declaration of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Declaration of Dist_norm
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

#ifndef Dist_norm_H
#define Dist_norm_H

#include "InferenceDistributionContinuous.h"
#include "RbValue.h"

#include <ostream>
#include <string>

class Dist_norm: public InferenceDistributionContinuous {

    public:
                                    Dist_norm(void);                                                    //!< Parser constructor

        // Basic utility functions
        Dist_norm*                  clone(void) const;                                                  //!< Clone object
        virtual void                setParameters(const std::vector<RbValue<void*> > &p);               //!< Set the pointers to the variables of the distribution. The last one is always the random value.

        // Normal distribution functions
        double                      cdf(double q);                                                      //!< Cumulative density
        double                      lnPdf(void) const;                                                  //!< Ln probability density
        double                      pdf(void) const;                                                    //!< Probability density
        double                      quantile(double p);                                                 //!< Quantile
        void                        rv(void);                                                           //!< Generate random variable
    
    private:

        // parameters
        RbValue<double*>            mu;
        RbValue<double*>            sigma;
    
        // return values
        RbValue<double*>            randomVariable;

};

#endif

