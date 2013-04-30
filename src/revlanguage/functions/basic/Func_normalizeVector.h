/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func_normalizeVector, which is used to create
 * value simplices.
 *
 * @brief Declaration and implementation of Func_normalizeVector
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-12 10:25:58 +0200 (Tue, 12 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func_normalizeVector.h 1626 2012-06-12 08:25:58Z hoehna $
 */

#ifndef Func_normalizeVector_H
#define Func_normalizeVector_H

#include "RlFunction.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_normalizeVector :  public Function {
        
    public:
        Func_normalizeVector();
        
        // Basic utility functions
        Func_normalizeVector*               clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RbLanguageObject*           execute(void);                                              //!< Execute function
        
    };
    
}


#endif


