/**
 * @file
 * This file contains the declaration of Func_normalize, which 
 * normalizes a vector of positive real numbers.
 *
 * @brief Implementation of Func_normalize
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
#include "Func_normalize.h"
#include "Integer.h"
#include "RbMath.h"
#include "RbException.h"
#include "RbNames.h"
#include "RealPos.h"
#include "ReferenceRule.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "VectorReal.h"

#include <cassert>
#include <cmath>

/** Clone object */
Func_normalize* Func_normalize::clone(void) const {

    return new Func_normalize(*this);
}


/** Execute function */
DAGNode* Func_normalize::executeOperation(const std::vector<DAGNode*>& args) {

    // Get first element
    std::vector<double> tempVec    = ((VectorRealPos*)(args[0]->getValue()))->getValue();
    double              desiredSum = ((RealPos*)(args[1]->getValue()))->getValue();
    
    // normalize the vector
    RbMath::normalize(tempVec, desiredSum);
        
    return new ConstantNode(new Simplex(tempVec));
}


/** Get argument rules */
const ArgumentRules& Func_normalize::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back(new ArgumentRule("", VectorRealPos_name));
        argumentRules.push_back(new ArgumentRule("", new RealPos(1.0)));
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_normalize::getClass(void) const {

    static VectorString rbClass = VectorString(Func_normalize_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_normalize::getReturnType(void) const {

    return Simplex_name;
}

