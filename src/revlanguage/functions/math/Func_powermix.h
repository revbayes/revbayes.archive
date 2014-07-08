/**
 * @file
 * This file contains the declaration of the
 * Func_powermix class, which is used to compute a
 * power mixture of two simplex parameters.
 *
 * @brief Declaration of Func_powermix
 *
 * (c) Copyright under GPL version 3
 */

#ifndef RevBayes_Func_powermix_h
#define RevBayes_Func_powermix_h

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    /**
     *   @brief Func_powermix
     *
     *   Function that takes two simplex parameters and mixes them in
     *   proportion to the powers of a two-component simplex.
     */
    class Func_powermix :  public Function {
        
    public:
        Func_powermix( void );
        
        // Basic utility functions
        Func_powermix*                                  clone(void) const;                                                              //!< Clone the object
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
