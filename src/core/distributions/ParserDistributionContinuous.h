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

#include "DistributionContinuous.h"
#include "MemberFunction.h"
#include "ParserDistribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;

class ParserDistributionContinuous: public ParserDistribution {
    
public:
    ParserDistributionContinuous(DistributionContinuous* d, const std::string &n, const MemberRules& memberRules, const RbPtr<RbLanguageObject> &rv);

    ParserDistributionContinuous(const ParserDistributionContinuous &p);
    virtual                                ~ParserDistributionContinuous(void) {}                                                   //!< Destructor
    
    // overloaded operators
    ParserDistributionContinuous&           operator=(const ParserDistributionContinuous& d);                                       //!< Assignment operator
    
    // Basic utility function
    virtual ParserDistributionContinuous*   clone(void) const;                                                                      //!< Clone object
    static const std::string&               getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;    
    
    // Member object functions
    double                                  cdf( const Real& value);                                                                //!< Cumulative probability
    DistributionContinuous*                 getLeanDistribution(void) const;                                                        //!< Get the lean distribution
    double                                  getMax(void) const;                                                                     //!< Get max value of coverage
    const MemberRules&                      getMemberRules(void) const;                                                             //!< Get member rules
    const MethodTable&                      getMethods(void) const;                                                                 //!< Get member methods
    double                                  getMin(void) const;                                                                     //!< Get min value of coverage
    double                                  jointLnPdf( const RbLanguageObject& value) const;                                       //!< Ln probability density
    double                                  lnPdf( const RbLanguageObject& value) const;                                            //!< Ln probability density
    double                                  pdf( const RbLanguageObject& value) const;                                              //!< Probability density
    double                                  quantile(const double p);                                                               //!< Quantile
    void                                    rv(void);                                                                               //!< Generate a random draw
    void                                    setParameters(const std::vector<RbValue<void*> > &p);                                   //!< Set the pointers to the variables of the distribution. The last one is always the random value.
    void                                    setValue(const RbValue<void*> &v);                                                      //!< Set the pointers to the value of the distribution.
    
protected:
    RbPtr<RbLanguageObject>                 executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args);     //!< Direct call of member method
    
    TypeSpec                                typeSpec;
    
    DistributionContinuous*                 distribution;
};

#endif

