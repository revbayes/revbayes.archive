/**
 * @file
 * This file contains the declaration of Dist_gamma, which is used to hold
 * parameters and functions related to an gamma distribution.
 *
 * @brief Declaration of Dist_gamma
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

#ifndef Dist_gamma_H
#define Dist_gamma_H

#include "DistributionContinuous.h"
#include "RealPos.h"

#include <ostream>
#include <string>

class DAGNode;
class StochasticNode;
class VectorString;

const std::string Dist_gamma_name = "Dist_gamma";

class Dist_gamma: public DistributionContinuous {

    public:
                                    Dist_gamma(void);                                                     //!< Parser constructor
                                    Dist_gamma(double shape, double rate);                                              //!< Internal constructor

        // Basic utility functions
        Dist_gamma*                   clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules

        // Exponential distribution functions
        double                      cdf(const RbLanguageObject* value);                                 //!< Cumulative density
        const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (RealPos)
        double                      lnPdf(const RbLanguageObject* value);                               //!< Ln probability density
        double                      pdf(const RbLanguageObject* value);                                 //!< Probability density
        RealPos*                    quantile(const double p);                                           //!< Quantile
        RealPos*                    rv(void);                                                           //!< Generate random variable
    
};

#endif

