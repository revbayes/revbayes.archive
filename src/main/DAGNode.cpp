#include "DAGNode.h"
#include "RbDataType.h"
#include <iostream>




DAGNode::DAGNode(RbDataType *dt) {

	value = dt;
	storedValue = dt->copy();
}

DAGNode::DAGNode(DAGNode &d) {

	delete value;
	delete storedValue;
}

DAGNode::~DAGNode(void) {

	if ( value != NULL )
		delete value;
	if ( storedValue != NULL )
		delete storedValue;
}

void DAGNode::printChildren(void) {

	if ( children.empty() )	
		{
		std::cout << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator c=children.begin(); c != children.end(); c++)
		std::cout << (*c) << " ";
	std::cout << std::endl;
}

void DAGNode::printParents(void) {

	if ( parents.empty() )
		{
		std::cout << std::endl;
		return;
		}
	for (std::set<DAGNode*>::iterator p=parents.begin(); p != parents.end(); p++)
		std::cout << (*p) << " ";
	std::cout << std::endl;
}
