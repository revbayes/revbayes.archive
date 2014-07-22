/**
 * @file
 * This file contains the declaration of Func_discretizeGamma,
 * which is used to normalize vectors, creating a
 * simplex.
 *
 * @brief Declaration of Func_discretizeGamma
 *
 * (c) Copyright 2009- under GPL version 3
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 */

#ifndef Func_discretizeGamma_H
#define Func_discretizeGamma_H

#include "RlFunction.h"
#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_discretizeGamma :  public Function {
        
    public:
        Func_discretizeGamma();
        
        // Basic utility functions
        Func_discretizeGamma*             clone(void) const;                                          //!< Clone the object
        static const std::string&   getClassName(void);                                         //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                  //!< Get type of return value
        
        
        RevObject*                  execute(void);                                              //!< Execute function
        
    };
    
}


#endif


