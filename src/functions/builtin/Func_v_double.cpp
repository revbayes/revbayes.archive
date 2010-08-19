/**
 * @file
 * This file contains the implementation of Func_v_double, which 
 * constructs a double vector from a list of doubles.
 *
 * @brief Implementation of Func_v_double
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
#include "Func_v_double.h"
#include "RbException.h"
#include "RbDouble.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Vector.h"
#include "WrapperRule.h"

#include <cassert>
#include <cmath>

/** Clone object */
Func_v_double* Func_v_double::clone(void) const {

    return new Func_v_double(*this);
}


/** Execute function */
RbObject* Func_v_double::executeOperation(const std::vector<DAGNode*>& args) {

    // Create temporary vector for the ints 
    std::vector<double>    tempVec;

    // Get first element
    tempVec.push_back(((RbDouble*)(args[0]->getValue()))->getValue());

    // Get following elements
    if ( args.size() > 1 ) {
        DAGNodeContainer*   elements = dynamic_cast<DAGNodeContainer*>(args[1]);
        for (size_t i=0; i<elements->size(); i++) {
            tempVec.push_back(((RbDouble*)(elements->getValElement(i)))->getValue());
        }
    }

    return new Vector(tempVec);
}


/** Get argument rules */
const ArgumentRules& Func_v_double::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back(new ArgumentRule("", RbDouble_name));
        argumentRules.push_back(new Ellipsis(RbDouble_name));
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const StringVector& Func_v_double::getClass(void) const {

    static StringVector rbClass = StringVector(Func_v_double_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_v_double::getReturnType(void) const {

    return Vector_name;
}

