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

#include "DistributionContinuous.h"
#include "Simplex.h"

#include <ostream>
#include <string>

class DAGNode;
class Real;
class Simplex;
class StochasticNode;
class VectorString;

class Dist_dirichlet: public DistributionContinuous {

    public:
                                    Dist_dirichlet(void);                                               //!< Parser constructor
                                    Dist_dirichlet(std::vector<double> a);                              //!< Internal constructor

        // Basic utility functions
        Dist_dirichlet*             clone(void) const;                                                  //!< Clone object
        const VectorString&         getClass(void) const;                                               //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules

        // Real-valued distribution functions
        double                      cdf(const RbObject* value);                                         //!< Cumulative density
        Move*                       getDefaultMove(StochasticNode* node);                               //!< Get default move
        const TypeSpec              getVariableType(void) const;                                        //!< Get random variable type (Simplex)
        double                      lnPdf(const RbObject* value);                                       //!< Ln probability density
        double                      pdf(const RbObject* value);                                         //!< Probability density
        Simplex*                    quantile(const double p);                                           //!< Quantile
        Simplex*                    rv(void);                                                           //!< Generate random variable
};

#endif

