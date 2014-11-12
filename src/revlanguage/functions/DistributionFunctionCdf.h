/**
 * @file
 * This file contains the declaration of the Cdf function of a distribution.
 *
 * @brief Declaration of DistributionFunctionCdf
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:40:37 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: ConstructorFunction.h 1602 2012-06-01 12:40:37Z hoehna $
 */

#ifndef DistributionFunctionCdf_H
#define DistributionFunctionCdf_H

#include "Probability.h"
#include "RlContinuousDistribution.h"
#include "RlTypedFunction.h"
#include "RlPositiveContinuousDistribution.h"

#include <string>
#include <vector>

namespace RevLanguage {
    
    class DistributionFunctionCdf : public TypedFunction<Probability> {
        
    public:
        DistributionFunctionCdf(ContinuousDistribution *d); 
        DistributionFunctionCdf(PositiveContinuousDistribution *d);                                                                     //!< Object constructor
        DistributionFunctionCdf(const DistributionFunctionCdf& obj);                                                                    //!< Copy constructor
        
        // overloaded operators
        DistributionFunctionCdf&                        operator=(const DistributionFunctionCdf& c);
        
        // Basic utility functions
        DistributionFunctionCdf*                        clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    protected:
        
        ArgumentRules                                   argRules;                                                                       //!< Member rules converted to reference rules
        ContinuousDistribution*                         templateObject;                                                                 //!< The template object
        PositiveContinuousDistribution*                 templateObjectPositive;                                                         //!< The template object
        
    };
    
}


#endif
