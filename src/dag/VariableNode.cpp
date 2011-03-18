/**
 * @file
 * This file contains the implementation of VariableNode, which is the base
 * class for variable nodes in the model DAG.
 *
 * @brief Implementation of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-13 15:50:43 +0100 (Sön, 13 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.cpp 188 2009-12-13 14:50:43Z ronquist $
 */

#include "Monitor.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>


/** Constructor */
VariableNode::VariableNode(const TypeSpec& valType)
    : DAGNode(valType) {

    touched      = false;
}


/** Get class vector describing type of object */
const VectorString& VariableNode::getClass() const {

    static VectorString rbClass = VectorString(VariableNode_name) + DAGNode::getClass();
    return rbClass;
}


/** Get default monitors */
std::vector<Monitor*> VariableNode::getDefaultMonitors(void) {

    std::vector<Monitor*>   monitors;
    monitors.push_back(new Monitor(this, 100));

    return monitors;
}

