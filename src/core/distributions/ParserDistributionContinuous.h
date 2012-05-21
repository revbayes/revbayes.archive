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
        
    // Basic utility function
    virtual ParserDistributionContinuous*   clone(void) const;                                                                      //!< Clone object
    static const std::string&               getClassName(void);                                                                     //!< Get class name
    static const TypeSpec&                  getClassTypeSpec(void);                                                                 //!< Get class type spec
    
    // Member object functions
    double                                  cdf( const Real& value);                                                                //!< Cumulative probability
    RbPtr<RbLanguageObject>                 executeSimpleMethod(const std::string& name, const std::vector<const RbObject*>& args); //!< Override to map member methods to internal functions
    double                                  getMax(void) const;                                                                     //!< Get max value of coverage
    const MethodTable&                      getMethods(void) const;                                                                 //!< Get member methods
    double                                  getMin(void) const;                                                                     //!< Get min value of coverage
    double                                  quantile(const double p);                                                               //!< Quantile
        
};

#endif

