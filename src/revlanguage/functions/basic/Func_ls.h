/**
 * @file
 * This file contains the declaration of Func_ls, which is
 * the function used to list the content of the workspace.
 *
 * @brief Declaration of Func_ls
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-22 12:06:58 -0700 (Sun, 22 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2012-09-07
 *
 * $Id: Func_source.h 1431 2012-04-22 19:06:58Z hoehna $
 */

#ifndef Func_ls_H
#define Func_ls_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_ls : public Procedure {
        
    public:
        Func_ls( void );
        
        // Basic utility functions
        Func_ls*                                clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        std::string                             getFunctionName(void) const;
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_ls functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

