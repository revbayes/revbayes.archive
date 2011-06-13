/**
 * @file
 * This file contains the declaration of Func_ln, which 
 * calculates the natural logarithm.
 *
 * @brief Declaration of Func_ln
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_ln_H
#define Func_ln_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_ln :  public RbFunction {
    
public:
    // Basic utility functions
    Func_ln*                    clone(void) const;                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                       //!< Get class vector
    
    // Regular functions
    DAGNode*                    execute(void);                                              //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
    
};

#endif

