/**
 * @file
 * This file contains the declaration of the templated Func_unot, which is used to to compare to values ( !a ).
 *
 * @brief Declaration unot implementation of Func__unot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_vector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func__unot_H
#define Func__unot_H

#include "RlFunction.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    class Func__unot : public Function {
        
    public:
        Func__unot();
        
        // Basic utility functions
        Func__unot*                 clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassType(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevPtr<Variable>            execute(void);                                              //!< Execute function
        
    };
    
}


#endif


