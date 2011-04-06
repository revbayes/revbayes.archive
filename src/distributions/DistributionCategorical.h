/**
 * @file
 * This file contains the declaration of DistributionCategorical, the
 * abstract base class for distributions on categorical variables.
 *
 * @brief Declaration of DistributionCategorical
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionCategorical.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef DistributionCategorical_H
#define DistributionCategorical_H

#include "Distribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class Move;
class Simplex;
class StochasticNode;
class VectorString;


class DistributionCategorical: public Distribution {

    public:
	    virtual                            ~DistributionCategorical(void) {}                                            //!< Destructor

        // Basic utility function
        virtual DistributionCategorical*    clone(void) const = 0;                                                          //!< Clone object
        virtual const VectorString&         getClass(void) const;                                                       //!< Get class vector   

        // Member object function you have to override
        virtual const MemberRules&          getMemberRules(void) const = 0;                                             //!< Get member rules

        // Member object functions you should not have to override
        const MethodTable&                  getMethods(void) const;                                                     //!< Get member methods
        DAGNode*                            executeMethod(const std::string& name, ArgumentFrame& args);                //!< Direct call of member method

        // Categorical distribution functions you have to override
        virtual Move*                       getDefaultMove(StochasticNode* node) = 0;                                   //!< Get default move
        virtual int                         getNumStates(void) const = 0;                                               //!< Get number of states
        virtual const Simplex*              getProbabilityMassVector(void) = 0;                                         //!< Get probability mass vector
        virtual const TypeSpec              getVariableType(void) const = 0;                                            //!< Get random variable type spec
        virtual double                      lnLikelihoodRatio(const RbObject* value) = 0;                               //!< Ln prob ratio of A | B when B is touched
        virtual double                      lnPdf(const RbObject* value) = 0;                                           //!< Ln probability density
        virtual double                      lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) = 0;           //!< Ln prob ratio of A | B when A is touched
        virtual double                      pdf(const RbObject* value) = 0;                                             //!< Probability density
        virtual RbObject*                   rv(void) = 0;                                                               //!< Generate a random draw

    protected:
                                            DistributionCategorical(const MemberRules& memberRules);                    //!< Constructor

};

#endif

