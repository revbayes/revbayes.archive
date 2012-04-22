/**
 * @file
 * This file contains the declaration of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Declaration of Dist_unif
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

#ifndef Dist_unif_H
#define Dist_unif_H

#include "InferenceDistributionContinuous.h"

#include <ostream>
#include <string>

class Dist_unif: public InferenceDistributionContinuous {

    public:
                                    Dist_unif(void);                                                    //!< constructor

        // Basic utility functions
        Dist_unif*                  clone(void) const;                                                  //!< Clone object

        // distribution functions
        double                      cdf(double q);                                                      //!< Cumulative density
        double                      getMax(void) const;                                                 //!< Get max value
        double                      getMin(void) const;                                                 //!< Get min value
        double                      quantile(double p);                                                 //!< Quantile
    
    private:
        double                      lnPdfSingleValue(std::vector<size_t> &offset) const;                //!< Ln probability density
        double                      pdfSingleValue(std::vector<size_t> &offset) const;                  //!< Probability density
        void                        rvSingleValue(std::vector<size_t> &offset);                         //!< Generate random variable
        void                        setInternalParameters(const std::vector<RbValue<void*> > &p);       //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    
        // parameters
        RbValue<double*>            min;
        RbValue<double*>            max;
    
        // return values
        RbValue<double*>            randomVariable;

};

#endif

