//
//  Func_cladoProbs.h
//  revbayes-proj
//
//  Created by Michael Landis on 1/19/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef Func_cladoProbs_H
#define Func_cladoProbs_H

#include "RlMatrixReal.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_cladoProbs : public TypedFunction<MatrixReal> {
        
    public:
        Func_cladoProbs( void );
        
        // Basic utility functions
        Func_cladoProbs*                                                clone(void) const;                                      //!< Clone the object
        static const std::string&                                       getClassType(void);                                     //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                 //!< Get class type spec
        std::string                                                     getFunctionName(void) const;                            //!< Get the primary name of the function in Rev
        const TypeSpec&                                                 getTypeSpec(void) const;                                //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::MatrixReal>*         createFunction(void) const;                             //!< Create internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                           //!< Get argument rules
        
    };
    
}


#endif /* defined(__revbayes_proj__Func_cladoProbs__) */
