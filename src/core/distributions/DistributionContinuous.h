/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for distributions on continuous variables.
 *
 * @brief Declaration of DistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-29 23:23:09 +0100 (Tis, 29 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: DistributionContinuous.h 211 2009-12-29 22:23:09Z ronquist $
 */

#ifndef DistributionContinuous_H
#define DistributionContinuous_H

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


const std::string DistributionContinuous_name = "Distribution on Continuous Random Variable";

class DistributionContinuous: public Distribution {

    public:
	    virtual                        ~DistributionContinuous(void) {}                                             //!< Destructor

        // Basic utility function
        virtual DistributionContinuous*         clone(void) const = 0;                                                      //!< Clone object
        virtual const VectorString&             getClass(void) const;                                                       //!< Get class vector   

        // Member object function you have to override
        virtual const RbPtr<MemberRules>        getMemberRules(void) const = 0;                                             //!< Get member rules

        // Member object functions you should not have to override
        const RbPtr<MethodTable>                getMethods(void) const;                                                     //!< Get member methods
        RbPtr<RbLanguageObject>                 executeOperation(const std::string& name, Environment& args);               //!< Direct call of member method

        // Interval distribution functions you probably want to override
        virtual const RbPtr<Real>               getMax(void) const;                                                         //!< Get max value of coverage
        virtual const RbPtr<Real>               getMin(void) const;                                                         //!< Get min value of coverage

        // Interval distribution functions you have to override
        virtual double                          cdf(const RbPtr<RbLanguageObject> value) = 0;                                     //!< Cumulative probability
        virtual const TypeSpec&                 getVariableType(void) const = 0;                                            //!< Get random variable type spec
        virtual double                          lnPdf(const RbPtr<RbLanguageObject> value) = 0;                                   //!< Ln probability density
        virtual double                          pdf(const RbPtr<RbLanguageObject> value) = 0;                                     //!< Probability density
        virtual RbPtr<Real>                     quantile(const double p) = 0;                                               //!< Quantile
        virtual RbPtr<RbLanguageObject>         rv(void) = 0;                                                               //!< Generate a random draw

    protected:
                                        DistributionContinuous(const RbPtr<MemberRules> memberRules);                     //!< Constructor

};

#endif

