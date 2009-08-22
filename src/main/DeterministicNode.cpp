#include "DeterministicNode.h"



DeterministicNode::DeterministicNode(RbDataType* dt) : DAGNode(dt) {

}

void DeterministicNode::print(void) {

	value->print();
	storedValue->print();
}