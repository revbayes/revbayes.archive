/**
 * @file
 * This file contains the declaration of Func_v_int, which 
 * constructs an int vector from a list of ints.
 *
 * @brief Declaration of Func_v_int
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_v_int_H
#define Func_v_int_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_v_int :  public RbFunction {

    public:
        // Basic utility functions
        Func_v_int*                 clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value

	protected:
        DAGNode*                    executeOperation(const std::vector<VariableSlot>& args);    //!< Execute operation
};

#endif

