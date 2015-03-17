//
//  Func_DECRateMatrix.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/16/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_DECRateMatrix__
#define __revbayes_proj__Func_DECRateMatrix__

#include "RlRateMatrix.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_DECRateMatrix : public TypedFunction<RateMatrix> {
        
    public:
        Func_DECRateMatrix( void );
        
        // Basic utility functions
        Func_DECRateMatrix*                                             clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateMatrix>*         createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif /* defined(__revbayes_proj__Func_DECRateMatrix__) */
