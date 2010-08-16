/**
 * @file
 * This file contains the declaration of Func_quit, which is
 * the function used to quit the program.
 *
 * @brief Declaration of Func_quit
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

#ifndef Func_quit_H
#define Func_quit_H

#include "RbFunction.h"

class Func_quit :  public RbFunction {

    public:
        // Basic utility functions
        RbObject*               clone(void) const;                  //!< Clone object
    	const StringVector&     getClass(void) const;               //!< Get class vector

        // Func_quit functions
        const ArgumentRules&    getArgumentRules(void) const;       //!< Get argument rules
        const std::string&      getReturnType(void) const;          //!< Get type of return val

	protected:
    	RbObject*               executeOperation(const std::vector<DAGNode*>& args);    //!< Execute function
};

#endif

