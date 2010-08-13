/**
 * @file
 * This file contains the implementation of WrapperRule, which is
 * used to describe rules for arguments passed by reference and
 * not by value.
 *
 * @brief Implementation of ArgumentRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbNames.h"
#include "StringVector.h"
#include "WrapperRule.h"

#include <string>


/** Constructor without default value */
WrapperRule::WrapperRule(const std::string& argName, const std::string& valType, int varDim)
    : ArgumentRule(argName, valType, varDim) {

    wrapperRule = true;
}
        
/** Constructor with default value */
WrapperRule::WrapperRule(const std::string& argName, DAGNode* defVar)
    : ArgumentRule(argName, defVar) {

    wrapperRule = true;
}

/** Get class vector describing type of object */
const StringVector& WrapperRule::getClass(void) const {

    static StringVector rbClass = StringVector(WrapperRule_name) + ArgumentRule::getClass();
    return rbClass;
}

