/**
 * @file
 * This file contains the declaration of Func_sin, which 
 * calculates the sin of x.
 *
 * @brief Declaration of Func_exp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_sin_H
#define Func_sin_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_sin :  public RbFunction {
    
public:
    // Basic utility functions
    Func_sin*                   clone(void) const;                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                       //!< Get class vector
    
    // Regular functions
    RbLanguageObject*           execute(void);                                              //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
    
};

#endif

