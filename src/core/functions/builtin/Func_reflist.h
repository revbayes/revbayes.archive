/**
 * @file
 * This file contains the declaration of Func_reflist,
 * which constructs a list of objects.
 *
 * @brief Declaration of Func_reflist
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_reflist_H
#define Func_reflist_H

#include "RbFunction.h"

class DAGNode;
class VectorString;


class Func_reflist :  public RbFunction {

    public:
        // Basic utility functions
        Func_reflist*                  clone(void) const;                                          //!< Clone the object
    	const VectorString&         getClass(void) const;                                       //!< Get class vector

        // Regular functions
    	DAGNode*                    execute(void);                                              //!< Execute function
        const ArgumentRules&        getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec              getReturnType(void) const;                                  //!< Get type of return value
};

#endif
