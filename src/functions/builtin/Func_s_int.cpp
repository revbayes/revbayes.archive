/**
 * @file
 * This file contains the implementation of Func_s_int, which 
 * constructs a simplex from a single integer.
 *
 * @brief Implementation of Func_s_int
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
#include "ConstantNode.h"
#include "DAGNode.h"
#include "DAGNodePlate.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "Func_s_int.h"
#include "Integer.h"
#include "RbMath.h"
#include "RbException.h"
#include "Real.h"
#include "RbNames.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "VectorReal.h"

#include <cassert>
#include <cmath>

/** Clone object */
Func_s_int* Func_s_int::clone(void) const {

    return new Func_s_int(*this);
}


/** Execute function */
DAGNode* Func_s_int::executeOperation(const std::vector<DAGNode*>& args) {


    // Get first element
    int simplexSize = ((Integer*)(args[0]->getValue()))->getValue();

    // check that the size is at least 2
    if (simplexSize < 2)
		throw (RbException("The simplex must have at least two elements"));
        
    // set up the default simplex
    std::vector<double> tempVec( simplexSize, 1.0/simplexSize );

    return new ConstantNode(new Simplex(tempVec));
}


/** Get argument rules */
const ArgumentRules& Func_s_int::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back(new ArgumentRule("", Integer_name));
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_s_int::getClass(void) const {

    static VectorString rbClass = VectorString(Func_s_int_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_s_int::getReturnType(void) const {

    return Simplex_name;
}

