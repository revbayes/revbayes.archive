/**
 * @file
 * This file contains the implementation of BuiltinFunction, which is
 * used to hold user-defined functions.
 *
 * @brief Implementation of BuiltinFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-21, version 1.0
 *
 * $Id$
 */

#include <list>

#include "ConstantNode.h"
#include "RbException.h"
#include "BuiltinFunction.h"


/** Basic constructor */
BuiltinFunction::BuiltinFunction(const ArgumentRule** rules, const std::string& retType,
                            RbObject* (*fxn)(std::vector<DAGNode*>))
    : RbFunction(rules, retType), executeFxn(fxn) {
}


/** Copy constructor */
BuiltinFunction::BuiltinFunction(const BuiltinFunction &fn)
    : RbFunction(fn), executeFxn(fn.executeFxn) {
}


/** Execute function */
RbObject* BuiltinFunction::execute(std::vector<DAGNode*> arguments) {

    return executeFxn(arguments);
}


