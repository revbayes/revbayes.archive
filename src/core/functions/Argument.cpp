/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id:$
 */

#include <sstream>

#include "Argument.h"
#include "DAGNode.h"
#include "RbNames.h"
#include "VectorString.h"


/** Construct from argument label and DAG node */
Argument::Argument(const std::string& argLabel, DAGNode* arg)
    : RbInternal() {

    label      = argLabel;
    dagNodePtr = arg;
}


/** Get class vector describing type of object */
const VectorString& Argument::getClass(void) const { 

    static VectorString rbClass = VectorString(Argument_name) + RbInternal::getClass();
	return rbClass; 
}


/** Complete info about object */
std::string Argument::richInfo(void) const {

    std::ostringstream o;
    o << "Argument: label = \"" << label << "\", value = ";
    dagNodePtr->printValue(o);

    return o.str();
}

