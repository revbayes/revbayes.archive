/**
 * @file
 * This file contains the declaration of Distribution, which specifies the
 * interface for distributions in RevBayes. A distribution is taken in
 * the statistical sense.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Distribution
 * @package distributions
 *
 * $Id: Distribution.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef Distribution_H
#define Distribution_H

#include "MemberObject.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class Move;
class RandomNumberGenerator;
class StochasticNode;
class VectorString;

/** @note Constructor adds random number generator to argument rules */
class Distribution: public MemberObject {

    public:
        virtual                    ~Distribution(void);                                                             //!< Destructor

        // Basic utility functions
        virtual const VectorString& getClass(void) const;                                                           //!< Get class vector   

        // Member variable rules and random variable type
        virtual const MemberRules&  getMemberRules(void) const;                                                     //!< Get member rules
        virtual const std::string&  getVariableType(void) const = 0;                                                //!< Get random variable type

        // Member method inits
        virtual const MethodTable&  getMethodInits(void) const;                                                     //!< Get method inits
        
        // Distribution functions
        virtual Move*               getDefaultMove(StochasticNode* node) = 0;                                       //!< Get default move
        virtual const RbObject*     getMax(void) { return NULL; }                                                   //!< Max value
        virtual const RbObject*     getMin(void) { return NULL; }                                                   //!< Min value
        virtual double              lnPriorRatio(const RbObject* newVal, const RbObject* oldVal) = 0;               //!< Ln prior ratio
        virtual double              lnPdf(const RbObject* value) = 0;                                               //!< Ln probability density function
        virtual double              lnLikelihoodRatio(const RbObject* value) = 0;                                   //!< Ln likelihood ratio
        virtual double              pdf(const RbObject* value) = 0;                                                 //!< Probability density function
        virtual RbObject*           rv(void) = 0;                                                                   //!< Generate a random draw

    protected:
									Distribution(const MemberRules& memberRules);                                   //!< Simple constructor
									Distribution(const MemberRules& memberRules, const MethodTable& methodInits);   //!< Inheritance constructor
									Distribution(const Distribution& x);                                            //!< Copy constructor

        // Assignment operator
        Distribution&               operator=(const Distribution& x);                                               //!< Assignment operator

        // Member method call
        virtual DAGNode*            executeOperation(const std::string& name, const std::vector<VariableSlot>& args);       //!< Execute method

        // Workspace variables
        Real                        retDouble;                                                                      //!< Return double
        RbObject*					retObject;                                                                      //!< Return object
};

#endif

