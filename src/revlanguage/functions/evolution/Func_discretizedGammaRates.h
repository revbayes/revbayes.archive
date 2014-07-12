/**
 * @file
 * This file contains the declaration of Func_discretizedGammaRates,
 * which returns a vector of normalized gamma rates for n categories
 * for among-site rate variation
 *
 * @brief Declaration of Func_discretizedGammaRates
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_discretizedGammaRates_H
#define Func_discretizedGammaRates_H

#include "RlFunction.h"
#include <map>
#include <string>
#include "TypedDagNode.h"

namespace RevLanguage {
    
    class Func_discretizedGammaRates :  public Function {
        
    public:
        Func_discretizedGammaRates();
        
        // Basic utility functions
        Func_discretizedGammaRates*             clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        RevObject*                  execute(void);                                              //!< Execute function
        
    };
    
}


#endif


