//
//  Func_cladoProbs.h
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_cladoProbs__
#define __revbayes_proj__Func_cladoProbs__

#include "RealMatrix.h"
#include "MatrixReal.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_cladoProbs : public TypedFunction<RealMatrix> {
        
    public:
        Func_cladoProbs( void );
        
        // Basic utility functions
        Func_cladoProbs*                                                clone(void) const;                                      //!< Clone the object
        static const std::string&                                       getClassType(void);                                     //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                 //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::MatrixReal>*         createFunction(void) const;                             //!< Create internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                           //!< Get argument rules
        
    };
    
}


#endif /* defined(__revbayes_proj__Func_cladoProbs__) */
