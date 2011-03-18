/**
 * @file
 * This file contains the implementation of Func_clamp, which is
 * the function used to clamp stochastic nodes to an observed value.
 *
 * @brief Implementation of Func_clamp
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

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_clamp.h"
#include "Integer.h"
#include "LookupNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "StochasticNode.h"
#include "VectorInteger.h"
#include "VectorString.h"

#include <cassert>

/** Clone object */
Func_clamp* Func_clamp::clone(void) const {

    return new Func_clamp(*this);
}


/** Execute function */
DAGNode* Func_clamp::executeOperation(const std::vector<DAGNode*>& args) {

    // Get the stochastic node from the variable lookup
    LookupNode* lookupNode = dynamic_cast<LookupNode*>(args[0]);
    assert(lookupNode != NULL);
    StochasticNode* theNode = dynamic_cast<StochasticNode*>(lookupNode->getVariable());
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

        argumentRules.push_back(new ReferenceRule("var", RbObject_name));
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

