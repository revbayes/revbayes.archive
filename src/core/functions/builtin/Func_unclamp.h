/**
 * @file
 * This file contains the declaration of Func_unclamp, which
 * is used to unclamp a stochastic node.
 *
 * @brief Declaration of Func_unclamp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_unclamp_H
#define Func_unclamp_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

const std::string Func_unclamp_name = "Setval function";

class Func_unclamp :  public RbFunction {

    public:
        // Basic utility functions
        Func_unclamp*               clone(void) const;                                  //!< Clone the object
        const VectorString&         getClass(void) const;                               //!< Get class vector

        // Regular functions
        bool                        addAsChildOfArguments(void) { return false; }       //!< We do not wish that this function is added as a child of the arguments
        RbLanguageObject*           execute(void);                                      //!< Execute operation
        const ArgumentRules&        getArgumentRules(void) const;                       //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                          //!< Get type of return value

};

#endif

