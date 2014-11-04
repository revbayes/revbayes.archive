/**
 * @file
 * This file contains the declaration of the RevLanguage minimum function, which
 * is used to created deterministic variable associated with the minimum function.
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

#include "RlTypedFunction.h"
#include "Real.h"

#include <string>

namespace RevLanguage {
    
    class Func_max :  public TypedFunction<Real> {
        
    public:
        Func_max( void );
        
        // Basic utility functions
        Func_max*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<double>*            createFunction(void) const;                                                     //!< Create internal function object
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
