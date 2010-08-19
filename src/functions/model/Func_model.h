/**
 * @file
 * This file contains the declaration of Func_model, which is
 * the function used to create models.
 *
 * @brief Declaration of Func_model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
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
        RbObject*               clone(void) const;                                      //!< Clone object
    	const VectorString&     getClass(void) const;                                   //!< Get class vector

        // Func_model functions
        const ArgumentRules&    getArgumentRules(void) const;                           //!< Get argument rules
        const std::string&      getReturnType(void) const;                              //!< Get type of return val

        // New design
        RbObject*               getValue(void);                                         //!< Get function value

	protected:
    	RbObject*               executeOperation(const std::vector<DAGNode*>& args);    //!< Execute function
};

#endif

