#include "Node.h"
#include "NodeFactory.h"



NodeFactory::~NodeFactory(void) {

	for (std::set<Node*>::iterator p=allocatedNodes.begin(); p != allocatedNodes.end(); p++)
		delete (*p);
}

void NodeFactory::drainNodePool(void) {

	for (std::vector<Node*>::iterator p=nodePool.begin(); p != nodePool.end(); p++)
		{
		allocatedNodes.erase( *p );
		delete (*p);
		}
}

Node* NodeFactory::getNode(void) { 

	if ( nodePool.empty() == true )
		{
		/* If the node pool is empty, we allocate a new node and return it. We
		   do not need to add it to the node pool. */
		Node *p = new Node;
		allocatedNodes.insert( p );
		return p;
		}
		
	// Return a node from the node pool, remembering to remove it from the pool.
	Node *p = nodePool.back();
	nodePool.pop_back();
	return p;
}

void NodeFactory::returnNodeToPool(Node* p) {

	p->clean();               // Make certain the information in the node is in some base state before returning it to the node pool.
	nodePool.push_back( p );
}
