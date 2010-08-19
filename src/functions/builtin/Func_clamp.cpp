/**
 * @file
 * This file contains the implementation of Func_clamp, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_clamp
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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func__lookup.h"
#include "Func_clamp.h"
#include "VectorInteger.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "WrapperRule.h"

#include <cassert>

/** Clone object */
Func_clamp* Func_clamp::clone(void) const {

    return new Func_clamp(*this);
}


/** Execute function */
RbObject* Func_clamp::executeOperation(const std::vector<DAGNode*>& args) {

    // Get the stochastic node from the variable lookup
    const DeterministicNode* lookupNode = dynamic_cast<const DeterministicNode*>(args[0]);
    assert(lookupNode != NULL);
    const Func__lookup* lookup = dynamic_cast<const Func__lookup*>(lookupNode->getFunction());
    assert (lookup != NULL);
    StochasticNode* theNode = dynamic_cast<StochasticNode*>(lookup->getVariable());
    if ( !theNode )
        throw ("The variable is not a stochastic node");
    
    theNode->clamp(args[1]->getValue()->clone());

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_clamp::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new WrapperRule("var", RbObject_name));
        argumentRules.push_back(new ArgumentRule("value", RbObject_name));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_clamp::getClass(void) const {

    static VectorString rbClass = VectorString(Func_clamp_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_clamp::getReturnType(void) const {

    return RbNULL_name;
}

