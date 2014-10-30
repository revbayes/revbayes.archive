/**
 * @file
 * This file contains the declaration of the RevLanguage logarithm function, which
 * is used to created deterministic variable associated with the logarithm function.
 *
 * @brief Declaration and implementation of Func_log
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_log_H
#define Func_log_H

#include "Real.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_log : public TypedFunction<Real> {
        
    public:
        Func_log( void );
        
        // Basic utility functions
        Func_log*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
