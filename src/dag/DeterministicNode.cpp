/**
 * @file
 * This file contains the implementation of DeterministicNode, which is a derived
 * class from DAGNode. DeterministicNode are DAG nodes with an expression assigned to its value.
 *
 * @brief Implementation of DeterministicNode
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

#include "DeterministicNode.h"
#include "../datatypes/RbDataType.h"
#include "../modelLanguage/parser/SyntaxElement.h"


DeterministicNode::DeterministicNode(RbDataType* dt, SyntaxElement* e) : DAGNode(dt) {
	rootSyntaxTree = e;
}

void DeterministicNode::print(void) {

	value->print();
	storedValue->print();
}
