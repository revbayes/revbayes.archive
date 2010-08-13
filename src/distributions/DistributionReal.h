/**
 * @file
 * This file contains the declaration of DistributionReal, which specifies the
 * interface for distributions on real-valued variables.
 *
 * @brief Declaration of DistributionReal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface DistributionReal
 * @package distributions
 *
 * $Id: DistributionReal.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef DistributionReal_H
#define DistributionReal_H

#include "Distribution.h"
#include "RbDouble.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Move;
class RandomNumberGenerator;
class StochasticNode;
class StringVector;

class DistributionReal: public Distribution {

    public:
	    virtual                     ~DistributionReal() {}                      //!< Destructor

        // Basic utility functions
        virtual const StringVector& getClass(void) const;                       //!< Get class vector   

        // Member variable rules and random variable type
        virtual const MemberRules&  getMemberRules(void) const = 0;             //!< Get member rules
        virtual const std::string&  getVariableType() const;                    //!< Get random variable type

        // Member method inits
        const MethodTable&  getMethodInits(void) const;                         //!< Get method inits
        
        // Real-valued distribution functions
        virtual double              cdf(const double q) = 0;                    //!< Cumulative probability
        virtual Move*               getDefaultMove(StochasticNode* node) = 0;   //!< Get default move
        virtual const RbDouble*     getMax(void);                               //!< Get max value
        virtual const RbDouble*     getMin(void);                               //!< Get min value
        virtual double              lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) = 0;  //!< Ln prior ratio
        virtual double              lnPdf(const RbObject* value) = 0;           //!< Ln probability density
        virtual double              lnLikelihoodRatio(const RbObject* value) = 0;   //!< Ln likelihood ratio
        virtual double              pdf(const RbObject* value) = 0;             //!< Probability density
        virtual double              quantile(const double p) = 0;               //!< Quantile
        virtual RbDouble*           rv() = 0;                                   //!< Generate a random draw

    protected:
                                    DistributionReal(const MemberRules& memberRules);   //!< Constructor

        // Member method call
        const RbObject*             executeOperation(const std::string&     name,
                                                     std::vector<DAGNode*>& args);      //!< Execute method
};

#endif

