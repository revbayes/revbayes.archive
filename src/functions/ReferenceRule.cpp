/**
 * @file
 * This file contains the implementation of ReferenceRule, which is
 * used to describe rules for arguments passed by reference and
 * not by value.
 *
 * @brief Implementation of ReferenceRule
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include "DAGNode.h"
#include "RbNames.h"
#include "ReferenceRule.h"
#include "VectorString.h"

#include <string>


/** Constructor without default value */
ReferenceRule::ReferenceRule(const std::string& argName, const std::string& valType, int valDim)
    : ArgumentRule(argName, TypeSpec(valType, valDim, true)) {
}
        
/** Constructor with default value */
ReferenceRule::ReferenceRule(const std::string& argName, DAGNode* defVar)
    : ArgumentRule(argName, TypeSpec(defVar->getValueType(), defVar->getDim(), true), defVar) {
}

/** Get class vector describing type of object */
const VectorString& ReferenceRule::getClass(void) const {

    static VectorString rbClass = VectorString(ReferenceRule_name) + ArgumentRule::getClass();
    return rbClass;
}

