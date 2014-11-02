/**
 * @file
 * This file contains the declaration of the RevLanguage fnAtlasGTR function, which
 * is used to created deterministic variable associated with the fnAtlasGTR function.
 *
 * @brief Declaration and implementation of Func_atlasEr
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z landis $
 */


#ifndef Func_atlasEr_H
#define Func_atlasEr_H

#include "RealPos.h"
#include "ModelVector.h"
#include "RlRateMatrix.h"
#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_atlasEr : public Procedure {
        
    public:
        Func_atlasEr( void );
        
        // Basic utility functions
        Func_atlasEr*                                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        const ArgumentRules&                                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                                 getReturnType(void) const;                                  //!< Get type of return value
        RevPtr<Variable>                                                execute(void);
    };
}

#endif
