/**
 * @file
 * This file contains the implementation of Func__range, which is
 * the function used to create models.
 *
 * @brief Implementation of Func__range
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
#include "Func__range.h"
#include "VectorInteger.h"
#include "Integer.h"
#include "RbException.h"
#include "RbNames.h"
#include "VectorString.h"


/** Clone object */
Func__range* Func__range::clone(void) const {

    return new Func__range(*this);
}


/** Execute function */
RbObject* Func__range::executeOperation(const std::vector<DAGNode*>& args) {

    int first = ((Integer*)(args[0])->getValue())->getValue();
    int last  = ((Integer*)(args[1])->getValue())->getValue();

    std::vector<int> temp;
    for (int i=first; i<=last; i++)
        temp.push_back(i);

    return new VectorInteger(temp);
}


/** Get argument rules */
const ArgumentRules& Func__range::getArgumentRules(void) const {

    static ArgumentRules argumentRules;
    static bool          rulesSet = false;

    if (!rulesSet) {

        argumentRules.push_back(new ArgumentRule("", RbInt_name));
        argumentRules.push_back(new ArgumentRule("", RbInt_name));
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class vector describing type of object */
const VectorString& Func__range::getClass(void) const {

    static VectorString rbClass = VectorString(Func__range_name) + RbFunction::getClass();
    return rbClass;
}


/** Get return dimension */
int Func__range::getReturnDim(void) const {

    return 1;
}


/** Get return type */
const std::string& Func__range::getReturnType(void) const {

    return VectorInteger_name;
}

