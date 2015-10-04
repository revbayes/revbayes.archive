//
//  Func_treeScale.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/6/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#ifndef Func_treeScalen_H
#define Func_treeScalen_H

#include "RlTimeTree.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_treeScale :  public TypedFunction<TimeTree> {
        
    public:
        Func_treeScale( void );
        
        // Basic utility functions
        Func_treeScale*                                                     clone(void) const;                                                              //!< Clone the object
        static const std::string&                                           getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                              getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                     getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<RevBayesCore::Tree>*                    createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                                                getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif