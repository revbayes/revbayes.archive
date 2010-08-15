/**
 * @file
 * This file contains the implementation of Func_model, which is
 * the function used to create models.
 *
 * @brief Implementation of Func_model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "DAGNode.h"
#include "Func_model.h"
#include "Model.h"
#include "RbException.h"
#include "RbNames.h"
#include "StringVector.h"
#include "Workspace.h"
#include "WrapperRule.h"


/** Clone object */
RbObject* Func_model::clone(void) const {

    return new Func_model(*this);
}

/** Convert to object of another class. The caller manages the object. */
RbObject* Func_model::convertTo(const std::string& type) const {

    return NULL;
}

/** Get argument rules */
const ArgumentRules& Func_model::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new WrapperRule("sinknode", RbObject_name));
        rulesSet = true;
    }

    return argumentRules;
}


/** Execute function */
const RbObject* Func_model::executeOperation(const std::vector<DAGNode*>& args) {

    if (args[0]->numChildren() != 0)
        throw RbException("Node is not a sink node");

    model = Model(args);

    return &model;
}


/** Get class vector describing type of object */
const StringVector& Func_model::getClass(void) const {

    static StringVector rbClass = StringVector(Func_model_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_model::getReturnType(void) const {

    return Model_name;
}


/** Get value */
RbObject* Func_model::getValue(void) {

    std::vector<DAGNode*> const & args = getProcessedArguments();

    if (args[0]->numChildren() != 0)
        throw RbException("Node is not a sink node");

    return new Model(args);
}

/** Convert to object of another class. The caller manages the object. */
bool Func_model::isConvertibleTo(const std::string& type) const {

    return false;
}

