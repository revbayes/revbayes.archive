/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for distributions on continuous variables.
 *
 * @brief Declaration of DistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
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

class DistributionContinuous: public Distribution {

    public:
	    virtual                        ~DistributionContinuous(void) {}                                                                 //!< Destructor

        // Basic utility function
        virtual DistributionContinuous*         clone(void) const = 0;                                                                  //!< Clone object
        static const std::string&               getClassName(void);                                                                     //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec

        // Member object function you have to override
        virtual const MemberRules&              getMemberRules(void) const = 0;                                                         //!< Get member rules

        // Member object functions you should not have to override
        const MethodTable&                      getMethods(void) const;                                                                 //!< Get member methods

        // Interval distribution functions you probably want to override
        virtual const Real&                     getMax(void) const;                                                                     //!< Get max value of coverage
        virtual const Real&                     getMin(void) const;                                                                     //!< Get min value of coverage

        // Interval distribution functions you have to override
        virtual double                          cdf( const RbLanguageObject& value) = 0;                                                //!< Cumulative probability
        virtual const TypeSpec&                 getVariableType(void) const = 0;                                                        //!< Get random variable type spec
        virtual double                          lnPdf( const RbLanguageObject& value) const = 0;                                        //!< Ln probability density
        virtual double                          pdf( const RbLanguageObject& value) const = 0;                                          //!< Probability density
        virtual const Real&                     quantile(const double p) = 0;                                                           //!< Quantile
        virtual const RbLanguageObject&         rv(void) = 0;                                                                           //!< Generate a random draw

    protected:
                                                DistributionContinuous( const MemberRules& memberRules);                                //!< Constructor
    const RbLanguageObject&                     executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Direct call of member method

    // memberfunction return values
    Real                                        min;
    Real                                        max;
    RealPos                                     cd;
    Real                                        quant;
};

#endif

