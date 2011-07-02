/**
 * @file
 * This file contains the declaration of Func_list,
 * which constructs a list of objects.
 *
 * @brief Declaration of Func_list
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_list_H
#define Func_list_H

#include "RbFunction.h"

class DAGNode;
class VectorString;


class Func_list :  public RbFunction {

    public:
        // Basic utility functions
        Func_list*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
};

#endif
