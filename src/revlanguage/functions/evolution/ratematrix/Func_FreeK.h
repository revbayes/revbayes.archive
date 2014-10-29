//
//  Func_FreeK.h
//  rb_mlandis
//
//  Created by Michael Landis on 9/19/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

/**
 * @file
 * This file contains the declaration of the RevLanguage Jukes-Cantor (JC) function, which
 * is used to create deterministic variable associated with the jc function.
 *
 * @brief Declaration and implementation of RlFunc_FreeK
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_FreeK_H
#define Func_FreeK_H

#include "RlTypedFunction.h"
#include "RlRateMatrix.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_FreeK :  public TypedFunction<RateMatrix> {
        
    public:
        Func_FreeK( void );
        
        // Basic utility functions
        Func_FreeK*                                                     clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction< RevBayesCore::RateMatrix >*        createFunction(void) const;                                                                  //!< Execute function
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        
    };
    
}

#endif
