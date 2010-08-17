/**
 * @file
 * This file contains the declaration of Dist_exp, which is used to hold
 * parameters and functions related to an exponential distribution.
 *
 * @brief Declaration of Dist_exp
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

#ifndef Dist_dirichlet_H
#define Dist_dirichlet_H

#include "Distribution.h"

#include <ostream>
#include <string>

class DAGNode;
class RbDouble;
class StochasticNode;
class StringVector;

class Dist_dirichlet: public Distribution {

    public:
                                    Dist_dirichlet(void);                                               //!< Parser constructor
                                    Dist_dirichlet(std::vector<double> a, RandomNumberGenerator* rng);  //!< Internal constructor

        // Basic utility functions
        Dist_dirichlet*             clone(void) const;                                                  //!< Clone object
        const StringVector&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        const std::string&          getVariableType(void) const;                                        //!< Get random variable type (PosReal)

        // Real-valued distribution functions
        double                      cdf(const double q);                                                //!< Cumulative density
        Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
        const RbDouble*             getMin(void);                                                       //!< Get min value
        double                      lnLikelihoodRatio(const RbObject* value);                           //!< Ln likelihood ratio
        double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
        double                      lnPriorRatio(const RbObject* newVal, const RbObject* oldVal);       //!< Ln prior ratio
        double                      pdf(const RbObject* value);                                         //!< Probability density
        double                      quantile(const double p);                                           //!< Quantile
        RbObject*                   rv(void);                                                           //!< Generate random variable
};

#endif

