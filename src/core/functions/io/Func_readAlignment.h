/**
 * @file
 * This file contains the declaration of Func_readAlignment, which uses the NCL to read in an alignment in any of the supported formats:
 * nexus
 *
 * @brief Declaration of Func_readAlignment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_readAlignment_H
#define Func_readAlignment_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_readAlignment :  public RbFunction {
    
public:
    // Basic utility functions
    Func_readAlignment*         clone(void) const;                                          //!< Clone the object
    const VectorString&         getClass(void) const;                                       //!< Get class vector
    
    // Regular functions
    DAGNode*                    execute(void);                                              //!< Execute function
    const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
    const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
    
};

#endif

