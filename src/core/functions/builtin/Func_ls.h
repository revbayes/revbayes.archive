/**
 * @file
 * This file contains the declaration of Func_ls, which is
 * the function used to list the content of the workspace.
 *
 * @brief Declaration of Func_ls
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id:$
 */

#ifndef Func_ls_H
#define Func_ls_H

#include "RbFunction.h"

class Func_ls :  public RbFunction {

    public:
        // Basic utility functions
        Func_ls*                clone(void) const;                  //!< Clone object
    	const VectorString&     getClass(void) const;               //!< Get class vector

        // Func_ls functions
    	DAGNode*                execute(void);                      //!< Execute function
        const ArgumentRules&    getArgumentRules(void) const;       //!< Get argument rules
        const TypeSpec          getReturnType(void) const;          //!< Get type of return val

};

#endif

