/**
 * @file
 * This file contains the declaration of Func_str, which is used
 * to print info about the structure of a variable.
 *
 * @brief Declaration of Func_str
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id:$
 */

#ifndef Func_str_H
#define Func_str_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_str :  public RbFunction {

    public:
        // Basic utility functions
        Func_str*                   clone(void) const;                      //!< Clone the object
    	const VectorString&         getClass(void) const;                   //!< Get class vector

        // Regular functions
        DAGNode*                    execute(void);                          //!< Execute operation
        const ArgumentRules&        getArgumentRules(void) const;           //!< Get argument rules
        const TypeSpec              getReturnType(void) const;              //!< Get type of return value

};

#endif

