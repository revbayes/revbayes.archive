/**
 * @file
 * This file contains the implementation of Func_v_int, which 
 * constructs an int vector from a list of ints.
 *
 * @brief Implementation of Func_v_int
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
#include "DAGNodeContainer.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func__lookup.h"
#include "Func_v_int.h"
#include "RbException.h"
#include "Integer.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "WrapperRule.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_v_int* Func_v_int::clone(void) const {

    return new Func_v_int(*this);
}


/** Execute function */
RbObject* Func_v_int::executeOperation(const std::vector<DAGNode*>& args) {

    // Create temporary vector for the ints 
    std::vector<int>    tempVec;

    // Get first element
    tempVec.push_back(((Integer*)(args[0]->getValue()))->getValue());

    // Get following elements
    if (args.size() > 1) {
        DAGNodeContainer*   elements = dynamic_cast<DAGNodeContainer*>(args[1]);
        for (size_t i=0; i<elements->size(); i++) {
            tempVec.push_back(((Integer*)(elements->getValElement(i)))->getValue());
        }
    }

    return new VectorInteger(tempVec);
}


/** Get argument rules */
const ArgumentRules& Func_v_int::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back(new ArgumentRule("", RbInt_name));
        argumentRules.push_back(new Ellipsis(RbInt_name));
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_v_int::getClass(void) const {

    static VectorString rbClass = VectorString(Func_v_int_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_v_int::getReturnType(void) const {

    return VectorInteger_name;
}

