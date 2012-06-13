/**
 * @file
 * This file contains the declaration of DistributiionContinuous, which specifies
 * the interface for RlDistributions on continuous variables.
 *
 * @brief Declaration of RlDistributionContinuous
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-19 09:25:05 +0100 (Mon, 19 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: RlDistributionContinuous.h 1353 2012-03-19 08:25:05Z hoehna $
 */

#ifndef RlDistributionContinuous_H
#define RlDistributionContinuous_H

#include "DistributionContinuous.h"
#include "RlDistribution.h"
#include "Real.h"

#include <set>
#include <string>

class ArgumentRule;

class RlDistributionContinuous: public RlDistribution {
    
public:
    RlDistributionContinuous(DistributionContinuous* d, const std::string &n, const MemberRules& memberRules, const RbPtr<RbLanguageObject> &rv);

    RlDistributionContinuous(const RlDistributionContinuous &p);
    virtual                                ~RlDistributionContinuous(void) {}                                                       //!< Destructor
        
    // Basic utility function
    virtual RlDistributionContinuous*       clone(void) const;                                                                      //!< Clone object
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

