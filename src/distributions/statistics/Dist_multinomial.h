/**
 * @file
 * This file contains the declaration of Dist_multinomial, which is used to hold
 * parameters and functions related to a multinomial distribution.
 *
 * @brief Declaration of Dist_multinomial
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

#ifndef Dist_multinomial_H
#define Dist_multinomial_H

#include "Distribution.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class StochasticNode;
class VectorString;

class Dist_multinomial: public Distribution {

    public:
                                    Dist_multinomial(void);                                               //!< Parser constructor
                                    Dist_multinomial(std::vector<double> a);                              //!< Internal constructor

        // Basic utility functions
        Dist_multinomial*           clone(void) const;                                                    //!< Clone object
        const VectorString&         getClass(void) const;                                                 //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                           //!< Get member variable rules
        const std::string&          getVariableType(void) const;                                          //!< Get random variable type (RealPos)

        // Real-valued distribution functions
        Move*                       getDefaultMove(StochasticNode* node);                                 //!< Get default move
        const Real*                 getMin(void);                                                         //!< Get min value
        double                      lnLikelihoodRatio(const RbObject* value);                             //!< Ln likelihood ratio
        double                      lnPdf(const RbObject* value);                                         //!< Ln probability density
        double                      lnPriorRatio(const RbObject* newVal, const RbObject* oldVal);         //!< Ln prior ratio
        double                      pdf(const RbObject* value);                                           //!< Probability density
        double                      quantile(const double p);                                             //!< Quantile
        RbObject*                   rv(void);                                                             //!< Generate random variable
};

#endif

