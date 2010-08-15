/**
 * @file
 * This file contains the declaration of Func_ls, which is
 * the function used to list the content of the workspace.
 *
 * @brief Declaration of Func_ls
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

#ifndef Func_ls_H
#define Func_ls_H

#include "RbFunction.h"

class Func_ls :  public RbFunction {

    public:
        // Basic utility functions
        RbObject*               clone(void) const;                  //!< Clone object
    	const StringVector&     getClass(void) const;               //!< Get class vector

        // Func_ls functions
        const ArgumentRules&    getArgumentRules(void) const;       //!< Get argument rules
        const std::string&      getReturnType(void) const;          //!< Get type of return val

	protected:
        RbObject*                   convertTo(const std::string& type) const;        //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;  //!< Is convertible to type and dim?
    	const RbObject*         executeOperation(const std::vector<DAGNode*>& args);    //!< Execute function
};

#endif

