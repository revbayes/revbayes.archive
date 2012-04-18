/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for ParserDistributions on continuous variables.
 *
 * @brief Declaration of ParserDistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: ParserDistributionContinuous.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef ParserDistributionContinuous_H
#define ParserDistributionContinuous_H

#include "MemberFunction.h"
#include "ParserDistribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;
class InferenceDistributionContinuous;

class ParserDistributionContinuous: public ParserDistribution {
    
public:
    ParserDistributionContinuous(InferenceDistributionContinuous* d, const MemberRules& memberRules, RbLanguageObject* rv);
    ParserDistributionContinuous(const ParserDistributionContinuous &p);
    virtual                                ~ParserDistributionContinuous(void) {}                                                   //!< Destructor
    
    // Basic utility function
    virtual ParserDistributionContinuous*   clone(void) const;                                                                      //!< Clone object
    static const std::string&               getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;    
    
    // Member object function you have to override
    virtual const MemberRules&              getMemberRules(void) const;                                                             //!< Get member rules
    
    // Member object functions you should not have to override
    const MethodTable&                      getMethods(void) const;                                                                 //!< Get member methods
    
    // Interval ParserDistribution functions you probably want to override
    virtual const Real&                     getMax(void) const;                                                                     //!< Get max value of coverage
    virtual const Real&                     getMin(void) const;                                                                     //!< Get min value of coverage
    
    // Interval ParserDistribution functions you have to override
    virtual double                          cdf( const Real& value);                                                                //!< Cumulative probability
    virtual const TypeSpec&                 getVariableType(void) const;                                                            //!< Get random variable type spec
    virtual double                          lnPdf( const RbLanguageObject& value) const;                                            //!< Ln probability density
    virtual double                          pdf( const RbLanguageObject& value) const;                                              //!< Probability density
    virtual double                          quantile(const double p);                                                               //!< Quantile
    virtual const RbLanguageObject&         rv(void);                                                                               //!< Generate a random draw
    
protected:
    const RbLanguageObject&                 executeOperationSimple(const std::string& name, const std::vector<Argument>& args);     //!< Direct call of member method
    
    // memberfunction return values
    Real                                    min;
    Real                                    max;
    RealPos                                 cd;
    Real                                    quant;
    
    InferenceDistributionContinuous*        distribution;
    MemberRules                             memberRules;
    RbLanguageObject*                       randomValue;
};

#endif

