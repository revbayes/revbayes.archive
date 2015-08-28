//
//  Func_DECRoot.h
//  revbayes-proj
//
//  Created by Michael Landis on 3/3/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__Func_DECRoot__
#define __revbayes_proj__Func_DECRoot__


#include "RlTypedFunction.h"
#include "RbVector.h"
#include "ModelVector.h"
#include "RealPos.h"
#include "RlSimplex.h"

#include <map>
#include <string>
#include <vector>

namespace RevLanguage {
    
    class Func_DECRoot :  public TypedFunction<Simplex> {
        
    public:
        Func_DECRoot( void );
        
        // Basic utility functions
        Func_DECRoot*                              clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >*  createFunction(void) const;                                                                  //!< Execute function
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif /* defined(__revbayes_proj__Func_DECRoot__) */
