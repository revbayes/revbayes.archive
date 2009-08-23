#include "Node.h"
#include "NodeFactory.h"
#include "Topology.h"


Topology::Topology(void) : RbDataType("Topology") {

}

Topology::Topology(int n) : RbDataType("Topology"){

}

Topology::Topology(Topology &t) : RbDataType("Topology") {

}

Topology::~Topology(void) {

	NodeFactory &f = NodeFactory::nodeFactoryInstance();
	for (std::vector<Node*>::iterator p=nodes.begin(); p != nodes.end(); p++)
		f.returnNodeToPool( *p );
}
