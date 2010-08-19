/**
 * @file
 * This file contains the declaration of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Declaration of Dist_unif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_unif_H
#define Dist_unif_H

#include "DistributionReal.h"

#include <ostream>
#include <string>

class RbDouble;
class VectorString;

class Dist_unif: public DistributionReal {

    public:
                                Dist_unif(void);                                 //!< Parser constructor
                                Dist_unif(double               min,
                                         double                max,
                                        RandomNumberGenerator* rng);            //!< Internal constructor

        // Basic utility functions
        Dist_unif*              clone(void) const;                              //!< Clone object
        const VectorString&     getClass(void) const;                           //!< Get class vector

        // Member variable setup
        const MemberRules&      getMemberRules(void) const;                     //!< Get member variable rules

        // Real-valued distribution functions
        double          cdf(const double q);                                    //!< Cumulative density
        Move*           getDefaultMove(StochasticNode* node);                   //!< Get default move
        const RbDouble* getMax(void);                                           //!< Get max value
        const RbDouble* getMin(void);                                           //!< Get min value
        double          lnLikelihoodRatio(const RbObject* value);               //!< Ln likelihood ratio
        double          lnPdf(const RbObject* value);                           //!< Ln probability density
        double          lnPriorRatio(const RbObject* newVal, const RbObject* oldVal); //!< Ln prior ratio
        double          pdf(const RbObject* value);                             //!< Probability density
        double          quantile(const double p);                               //!< Quantile
        RbDouble*       rv();                                                   //!< Generate random variable
};

#endif

