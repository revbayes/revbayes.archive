/**
 * @file
 * This file contains the declaration of Dist_norm, which is used to hold
 * parameters and functions related to a normal distribution.
 *
 * @brief Declaration of Dist_norm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Dist_norm_H
#define Dist_norm_H

#include "DistributionContinuous.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class VectorString;

class Dist_norm: public DistributionContinuous {

    public:
                                    Dist_norm(void);                                                    //!< Parser constructor
                                    Dist_norm(double mu, double sigma);                                 //!< Internal constructor

        // Basic utility functions
        Dist_norm*                  clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules

        // Normal distribution functions
        double                      cdf(const RbObject* value);                                         //!< Cumulative density
        Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
        const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (RealPos)
        double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
        double                      pdf(const RbObject* value);                                         //!< Probability density
        Real*                       quantile(const double p);                                           //!< Quantile
        Real*                       rv(void);                                                           //!< Generate random variable
};

#endif

