/**
 * @file
 * This file contains the declaration of Distribution, which specifies the
 * interface for distributions in RevBayes. A distribution is taken in
 * the statistical sense.
 *
 * @brief Declaration of Distribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 * @interface Distribution
 * @package distributions
 *
 * $Id$
 */

#ifndef Distribution_H
#define Distribution_H

#include "MemberObject.h"
#include "StringVector.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class RandomNumberGenerator;
class StringVector;

class Distribution: public MemberObject {

    public:
	    virtual                         ~Distribution() {}                          //!< Destructor

        // Basic utility functions
        virtual const StringVector&     getClass(void) const;                       //!< Get class vector   

        // Member rules and variable type
        virtual const ArgumentRule**    getMemberRules(void) = 0;                   //!< Get member rules
        virtual const std::string       getReturnType() const = 0;                  //!< Get variable type

        // Distribution functions
        virtual double                  lnPdfRatio(const RbObject* newVal, const RbObject* oldVal) = 0;  //!< Calculate ln prior ratio
        virtual double                  lnPdf(const RbObject* value) = 0;           //!< Ln probability density function
//        virtual double                  lnLikelihoodRatio(const RbObject* value) = 0;    //!< Calculate ln likelihood ratio
        virtual double                  pdf(const RbObject* value) = 0;             //!< Probability density function
        virtual RbObject*               rv()  = 0;                                  //!< Generate a random draw

    protected:
                                        Distribution(const ArgumentRule** memberRules, RandomNumberGenerator* r); //!< Constructor

        RandomNumberGenerator*          rng;
};

#endif

