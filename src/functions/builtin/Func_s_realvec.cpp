/**
 * @file
 * This file contains the implementation of Func_s_realvec, which 
 * constructs a simplex from a VectorReal.
 *
 * @brief Implementation of Func_s_realvec
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
#include "Func_s_realvec.h"
#include "RbMath.h"
#include "RbException.h"
#include "Real.h"
#include "RbNames.h"
#include "Simplex.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "VectorReal.h"
#include "WrapperRule.h"

#include <cassert>
#include <cmath>

/** Clone object */
Func_s_realvec* Func_s_realvec::clone(void) const {

    return new Func_s_realvec(*this);
}


/** Execute function */
RbObject* Func_s_realvec::executeOperation(const std::vector<DAGNode*>& args) {

    // Get the vector of real numbers 
    std::vector<double> tempVec = ((VectorRealPos*)(args[0]->getValue()))->getValue();
        
    // check that the elements sum to 1.0
    double sum = 0.0;
    for (size_t i=0; i<tempVec.size(); i++)
        sum += tempVec[i];
    if ( RbMath::isEqualTo(sum, 1.0, 0.0000001) == false )
		throw (RbException("The values for the simple do not sum to one"));
        
    // renormalization, just to be really, really safe
    RbMath::normalize(tempVec, 1.0);

    return new Simplex(tempVec);
}


/** Get argument rules */
const ArgumentRules& Func_s_realvec::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
        argumentRules.push_back(new ArgumentRule("", VectorRealPos_name));
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func_s_realvec::getClass(void) const {

    static VectorString rbClass = VectorString(Func_s_realvec_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const std::string& Func_s_realvec::getReturnType(void) const {

    return Simplex_name;
}

