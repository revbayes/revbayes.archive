/**
 * @file
 * This file contains the implementation of StochasticNode, which is a derived
 * class from DAGNode. StochasticNodes are DAG nodes with an distribution assigned to its value.
 *
 * @brief Implementation of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#include "../datatypes/RbDataType.h"
#include "StochasticNode.h"



StochasticNode::StochasticNode(void) : DAGNode(NULL) {

	distribution = NULL;
}

StochasticNode::StochasticNode(RbDataType *dat, Distribution *dist) : DAGNode(dat) {

	distribution = dist;
}

StochasticNode::~StochasticNode(void) {

	if ( distribution != NULL )
		delete distribution;
}

void StochasticNode::print(void) {

	value->print();
	storedValue->print();
}
