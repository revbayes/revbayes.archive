/**
 * @file
 * This file contains the implementation of Func_model, which is
 * the function used to create models.
 *
 * @brief Implementation of Func_model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "Func_model.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "VectorString.h"
#include "Workspace.h"


/** Clone object */
RbObject* Func_model::clone(void) const {

    return new Func_model(*this);
}


/** Execute function */
DAGNode* Func_model::executeOperation(const std::vector<DAGNode*>& args) {

    if (args[0]->numChildren() != 0)
        throw RbException("Node is not a sink node");

    return new ConstantNode(new Model(args));
}


/** Get argument rules */
const ArgumentRules& Func_model::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new ReferenceRule("sinknode", RbObject_name));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_model::getClass(void) const {

    static VectorString rbClass = VectorString(Func_model_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_model::getReturnType(void) const {

    return Model_name;
}

