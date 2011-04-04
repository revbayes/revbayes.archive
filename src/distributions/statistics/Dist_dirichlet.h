/**
 * @file
 * This file contains the declaration of Dist_dirichlet, which is used to hold
 * parameters and functions related to a Dirichlet distribution.
 *
 * @brief Declaration of Dist_dirichlet
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

#ifndef Dist_dirichlet_H
#define Dist_dirichlet_H

#include "Distribution.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class StochasticNode;
class VectorString;

class Dist_dirichlet: public Distribution {

    public:
                                    Dist_dirichlet(void);                                               //!< Parser constructor
                                    Dist_dirichlet(std::vector<double> a);                              //!< Internal constructor

        // Basic utility functions
        Dist_dirichlet*             clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        const std::string&          getVariableType(void) const;                                        //!< Get random variable type (RealPos)

        // Real-valued distribution functions
        double                      cdf(const double q);                                                //!< Cumulative density
        Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
        const Real*                 getMin(void);                                                       //!< Get min value
        double                      lnLikelihoodRatio(const RbObject* value);                           //!< Ln prob ratio of A | B when only B is touched
        double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
        double                      lnPriorRatio(const RbObject* newVal, const RbObject* oldVal);       //!< Ln prob ratio of A | B when only A is touched
        double                      lnProbabilityRatio(const RbObject* newVal, const RbObject* oldVal); //!< Ln prob ratio of A | B when both A and B are touched
        double                      pdf(const RbObject* value);                                         //!< Probability density
        double                      quantile(const double p);                                           //!< Quantile
        RbObject*                   rv(void);                                                           //!< Generate random variable
};

#endif

