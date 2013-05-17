/**
 * @file
 * This file contains the declaration of ConstructorCtmcModel, which is used to construct a distribution object
 * of the CTMCModel in various different flavours.
 *
 * @brief Declaration of ConstructorCtmcModel
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-06-01 14:40:37 +0200 (Fri, 01 Jun 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: ConstructorFunction.h 1602 2012-06-01 12:40:37Z hoehna $
 */

#ifndef ConstructorCtmcModel_H
#define ConstructorCtmcModel_H

#include "RlFunction.h"

#include <string>

namespace RevLanguage {
    
    class ConstructorCtmcModel :  public Function {
        
    public:
        ConstructorCtmcModel(void);                                                                                             //!< Object constructor
        
        
        // Basic utility functions
        ConstructorCtmcModel*                   clone(void) const;                                                              //!< Clone the object
        static const std::string&               getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                        //!< Get language type of the object
        
        // Regular functions
        RbLanguageObject*                       execute(void);                                                                  //!< Execute the function. This is the function one has to overwrite for single return values.
        const ArgumentRules&                    getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                      //!< Get type of return value
                        
    };
    
}

#endif
