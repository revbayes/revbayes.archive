#include "RbDataType.h"
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