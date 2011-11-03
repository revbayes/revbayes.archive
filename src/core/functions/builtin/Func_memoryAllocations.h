/**
 * @file
 * This file contains the declaration of Func_memoryAllocations, which is
 * the function used to list the content of the workspace.
 *
 * @brief Declaration of Func_memoryAllocations
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

#ifndef Func_memoryAllocations_H
#define Func_memoryAllocations_H

#include "RbFunction.h"

const std::string Func_memoryAllocations_name = "memoryAllocations function";

class Func_memoryAllocations :  public RbFunction {
    
public:
    // Basic utility functions
    Func_memoryAllocations*     clone(void) const;                                  //!< Clone object
    const VectorString&         getClass(void) const;                               //!< Get class vector
    const TypeSpec&             getTypeSpec(void) const;                            //!< Get language type of the object
    
    // Func_memoryAllocations functions
    RbPtr<RbLanguageObject>     execute(void);                                      //!< Execute function
    RbPtr<const ArgumentRules>  getArgumentRules(void) const;                       //!< Get argument rules
    const TypeSpec&             getReturnType(void) const;                          //!< Get type of return val
    
private:
    static const TypeSpec       typeSpec;
    static const TypeSpec       returnTypeSpec;
};

#endif

