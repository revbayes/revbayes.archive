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

#include "RlContinuousDistribution.h"
#include "RlFunction.h"
#include "RlPositiveContinuousDistribution.h"

#include <string>
#include <vector>

namespace RevLanguage {
    
    class DistributionFunctionCdf :  public Function {
        
    public:
        DistributionFunctionCdf(ContinuousDistribution *d); 
        DistributionFunctionCdf(PositiveContinuousDistribution *d);                                                                               //!< Object constructor
        DistributionFunctionCdf(const DistributionFunctionCdf& obj);                                                                    //!< Copy constructor
        
        // overloaded operators
        DistributionFunctionCdf&                operator=(const DistributionFunctionCdf& c);
        
        // Basic utility functions
        DistributionFunctionCdf*                clone(void) const;                                                              //!< Clone the object
        static const std::string&               getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Regular functions
        RevObject*                              execute(void);                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
        const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
        
    protected:
        
        ArgumentRules                           argRules;                                                                       //!< Member rules converted to reference rules
        ContinuousDistribution*                 templateObject;                                                                 //!< The template object
        PositiveContinuousDistribution*         templateObjectPositive;                                                                 //!< The template object
        
    };
    
}


#endif
