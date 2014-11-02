/**
 * @file
 * This file contains the declaration of the RevLanguage maximum function, which
 * is used to created deterministic variable associated with the maximum function.
 *
 * @brief Declaration and implementation of Func_max
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_max.h 1406 2014-08-27 02:06:14Z boussau $
 */


#ifndef Func_max_H
#define Func_max_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    class Func_max :  public Function {
        
    public:
        Func_max( void );
        
        // Basic utility functions
        Func_max*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevPtr<Variable>                                execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
