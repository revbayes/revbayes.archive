/**
 * @file
 * This file contains the declaration of DistributionInterval, which specifies
 * the interface for distributions on interval variables.
 *
 * @brief Declaration of DistributionInterval
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionInterval.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef DistributionInterval_H
#define DistributionInterval_H

#include "Distribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Move;
class RandomNumberGenerator;
class StochasticNode;
class VectorString;


class DistributionInterval: public Distribution {

    public:
	    virtual                        ~DistributionInterval(void) {}                                              //!< Destructor

        // Basic utility function
        virtual DistributionInterval*   clone(void) const = 0;                                                          //!< Clone object
        virtual const VectorString&     getClass(void) const;                                                      //!< Get class vector   

        // Member object function you have to override
        virtual const MemberRules&      getMemberRules(void) const = 0;                                             //!< Get member rules

        // Member object functions you should not have to override
        const MethodTable&              getMethods(void) const;                                                     //!< Get member methods
        DAGNode*                        executeMethod(const std::string& name, ArgumentFrame& args);                //!< Direct call of member method

        // Interval distribution functions you probably want to override
        virtual const RbObject*         getMax(void);                                                               //!< Get max value of coverage
        virtual const RbObject*         getMin(void);                                                               //!< Get min value of coverage

        // Interval distribution functions you have to override
        virtual double                  cdf(const RbObject* value) = 0;                                             //!< Cumulative probability
        virtual Move*                   getDefaultMove(StochasticNode* node) = 0;                                   //!< Get default move
        virtual const TypeSpec          getVariableType(void) const = 0;                                            //!< Get random variable type spec
        virtual double                  lnLikelihoodRatio(const RbObject* value) = 0;                               //!< Ln prob ratio of A | B when B is touched
        virtual double                  lnPdf(const RbObject* value) = 0;                                           //!< Ln probability density
        virtual double                  lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) = 0;           //!< Ln prob ratio of A | B when A is touched
        virtual double                  pdf(const RbObject* value) = 0;                                             //!< Probability density
        virtual RbObject*               quantile(const double p) = 0;                                               //!< Quantile
        virtual RbObject*               rv(void) = 0;                                                               //!< Generate a random draw

    protected:
                                        DistributionInterval(const MemberRules& memberRules);                       //!< Constructor

};

#endif

