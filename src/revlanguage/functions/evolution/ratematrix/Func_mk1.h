/**
 * @file
 * This file contains the declaration of the RevLanguage mk1 function, which
 * is used to created a deterministic variable associated with the mk1 function.
 *
 * @brief Declaration and implementation of Chromosomes_pomo
 *
 * (c) Copyright 2014- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 */


#ifndef Func_mk1_H
#define Func_mk1_H

#include "RlRateMatrix.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_mk1 :  public TypedFunction<RateMatrix> {
        
    public:
        Func_mk1( void );
        
        // Basic utility functions
        Func_mk1*                                                    clone(void) const;                                                              //!< Clone the object
        static const std::string&                                    getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                       getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                              getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateMatrix>*      createFunction(void) const;                                                                  //!< Execute function
        const ArgumentRules&                                         getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif