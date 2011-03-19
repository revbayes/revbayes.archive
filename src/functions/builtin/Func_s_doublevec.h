/**
 * @file
 * This file contains the declaration of Func_s, which 
 * constructs a simplex from a list of doubles.
 *
 * @brief Declaration of Func_v_double
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_s_doublevec_H
#define Func_s_doublevec_H

#include "RbFunction.h"

#include <map>
#include <string>
#include <vector>

class DAGNode;
class VectorString;

class Func_s_doublevec :  public RbFunction {

    public:
        // Basic utility functions
        Func_s_doublevec*           clone(void) const;                                       //!< Clone the object
    	const VectorString&         getClass(void) const;                                    //!< Get class vector

        // Regular functions
        const ArgumentRules&        getArgumentRules(void) const;                            //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                               //!< Get type of return value

	protected:
        DAGNode*                    executeOperation(const std::vector<DAGNode*>& args);     //!< Execute operation
};

#endif

