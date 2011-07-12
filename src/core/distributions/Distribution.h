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


class Distribution: public MemberObject {

    public:
        virtual                    ~Distribution(void) {}                                                           //!< Destructor

        // Basic utility functions
        virtual Distribution*       clone(void) const = 0;                                                          //!< Clone object
        virtual const VectorString& getClass(void) const;                                                           //!< Get class vector   

        // Member object function you have to override
        virtual const MemberRules&  getMemberRules(void) const = 0;                                                 //!< Get member rules

        // Member object functions you may want to override
        virtual const MethodTable&  getMethods(void) const;                                                         //!< Get member methods
        virtual DAGNode*            executeOperation(const std::string& name, ArgumentFrame& args);                 //!< Map member methods to internal functions


        // Distribution functions you have to override
        virtual Move*               getDefaultMove(StochasticNode* node) = 0;                                       //!< Get default move
        virtual const TypeSpec      getVariableType(void) const = 0;                                                //!< Get random variable type
        virtual double              lnPdf(const RbObject* value) = 0;                                               //!< Ln probability density
        virtual double              pdf(const RbObject* value) = 0;                                                 //!< Probability density function
        virtual RbObject*           rv(void) = 0;                                                                   //!< Generate a random draw

    protected:
									Distribution(const MemberRules& memberRules);                                   //!< Simple constructor

};

#endif

