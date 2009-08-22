#include "Node.h"



Node::Node(void) {

	clean();
}

void Node::clean(void) {

	parent = NULL;
	children.clear();
	name = "";
}
