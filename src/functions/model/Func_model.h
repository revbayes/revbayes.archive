/**
 * @file
 * This file contains the declaration of Func_model, which is
 * the function used to create models.
 *
 * @brief Declaration of Func_model
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
 * $Id$
 */

#ifndef Func_model_H
#define Func_model_H

#include "Model.h"
#include "RbFunction.h"

class Func_model :  public RbFunction {

    public:
        // Basic utility functions
        Func_model*             clone(void) const;                                          //!< Clone object
    	const VectorString&     getClass(void) const;                                       //!< Get class vector

        // Func_model functions
    	DAGNode*                executeFunction(void);                                      //!< Execute function
        const ArgumentRules&    getArgumentRules(void) const;                               //!< Get argument rules
        const TypeSpec          getReturnType(void) const;                                  //!< Get type of return val

};

#endif

