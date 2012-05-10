/**
 * @file
 * This file contains the declaration of Func_clear, which is
 * the function used to clear the content of the workspace.
 *
 * @brief Declaration of Func_clear
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#ifndef Func_clear_H
#define Func_clear_H

#include "RbFunction.h"

class Func_clear :  public RbFunction {
    
public:
    // Basic utility functions
    Func_clear*                 clone(void) const;                                  //!< Clone object
    static const std::string&   getClassName(void);                                 //!< Get class name
    static const TypeSpec&      getClassTypeSpec(void);                             //!< Get class type spec
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
    
    // Func_clear functions
    RbPtr<RbLanguageObject>     execute(void);                                      //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                          //!< Get type of return val

protected:

};

#endif

