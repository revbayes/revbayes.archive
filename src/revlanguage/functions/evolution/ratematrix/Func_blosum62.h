/**
 * @file
 * This file contains the declaration of the RevLanguage Blosum62 function, which
 * is used to create deterministic variable associated with the Blosum62 function.
 *
 * @brief Declaration and implementation of Func_blosum62
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 */


#ifndef Func_blosum62_H
#define Func_blosum62_H

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_blosum62 :  public Function {
        
    public:
        Func_blosum62( void );
        
        // Basic utility functions
        Func_blosum62*                                  clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
