/**
 * @file
 * This file contains the declaration of Func_v_double, which 
 * constructs a double vector from a list of doubles.
 *
 * @brief Declaration of Func_v_double
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_v_double_H
#define Func_v_double_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class StringVector;

class Func_v_double :  public RbFunction {

    public:
        // Basic utility functions
        Func_v_double*              clone(void) const;                                       //!< Clone the object
    	const StringVector&         getClass(void) const;                                    //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                            //!< Get argument rules
        const std::string&          getReturnType(void) const;                               //!< Get type of return value

	protected:
        RbObject*                   executeOperation(const std::vector<DAGNode*> & args);    //!< Execute operation
};

#endif

