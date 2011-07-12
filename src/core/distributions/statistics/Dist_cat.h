/**
 * @file
 * This file contains the declaration of Dist_cat, which is used to hold
 * parameters and functions related to a distribution on a categorical
 * variable ( a factor).
 *
 * @brief Declaration of Dist_cat
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionCategorical.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef Dist_cat_H
#define Dist_cat_H

#include "Categorical.h"
#include "DistributionDiscrete.h"

#include <set>
#include <string>

class ArgumentRule;
class Categorical;
class Move;
class Simplex;
class StochasticNode;
class VectorString;


class Dist_cat: public DistributionDiscrete {

    public:
                                    Dist_cat(void);                                                         //!< Parser constructor
                                    Dist_cat(std::vector<double> m);                                        //!< Internal constructor
        virtual                    ~Dist_cat(void) {}                                                       //!< Virtual destructor

        // Basic utility functions
        Dist_cat*                   clone(void) const;                                                      //!< Clone object
        const VectorString&         getClass(void) const;                                                   //!< Get class vector

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                             //!< Get member variable rules

        // Discrete distribution functions
        Move*                       getDefaultMove(StochasticNode* node);                                   //!< Get default move
        virtual int                 getNumStates(void) const;                                               //!< Get number of states
        virtual const Simplex*      getProbabilityMassVector(void);                                         //!< Get probability mass vector
        const TypeSpec              getVariableType(void) const;                                            //!< Get random variable type (Simplex)
        double                      lnPdf(const RbObject* value);                                           //!< Ln probability density
        double                      pdf(const RbObject* value);                                             //!< Probability density
        Categorical*                rv(void);                                                               //!< Generate random variable

};

#endif

