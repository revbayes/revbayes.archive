/**
 * @file
 * This file contains the declaration of the BppO Model function.
 *
 * @brief Declaration and implementation of Func_BppModel
 *
 * (c) Copyright under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id: Func_BppModel.h 1406 2014-07-11 gueguen $
 */


#ifndef Func_BppModel_H
#define Func_BppModel_H

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_BppModel :  public Function {
        
    public:
        Func_BppModel( void );
        
        // Basic utility functions
        Func_BppModel*                                       clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
