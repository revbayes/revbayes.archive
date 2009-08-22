#include "Node.h"
#include "NodeFactory.h"
#include "Topology.h"


Topology::Topology(void) {

}

Topology::Topology(int n) {

}

Topology::Topology(Topology &t) {

}

Topology::~Topology(void) {

	NodeFactory &f = NodeFactory::nodeFactoryInstance();
	for (std::vector<Node*>::iterator p=nodes.begin(); p != nodes.end(); p++)
		f.returnNodeToPool( *p );
}
