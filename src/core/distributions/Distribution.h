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

#include "ConstantMemberObject.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class DAGNode;
class RandomNumberGenerator;
class StochasticNode;
class VectorString;


const std::string Distribution_name = "Distribution";

class Distribution: public ConstantMemberObject {

    public:
        virtual                    ~Distribution(void) {}                                                                       //!< Destructor

        // Basic utility functions
        virtual Distribution*               clone(void) const = 0;                                                              //!< Clone object
        virtual const VectorString&         getClass(void) const;                                                               //!< Get class vector   

        // Member object function you have to override
        virtual const MemberRules*          getMemberRules(void) const = 0;                                                     //!< Get member rules

        // Member object functions you may want to override
        virtual const MethodTable*          getMethods(void) const;                                                             //!< Get member methods


        // Distribution functions you have to override
        virtual const TypeSpec&             getVariableType(void) const = 0;                                                    //!< Get random variable type
        virtual double                      lnPdf( const RbLanguageObject *value) const = 0;                                    //!< Ln probability density
        virtual double                      pdf( const RbLanguageObject *value) const = 0;                                      //!< Probability density function
        virtual RbLanguageObject*           rv(void) = 0;                                                                       //!< Generate a random draw

    protected:
                                            Distribution( const MemberRules* memberRules);                                      //!< Simple constructor

        virtual RbLanguageObject*           executeOperationSimple(const std::string& name, Environment* args);                 //!< Map member methods to internal functions

};

#endif

