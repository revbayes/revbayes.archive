#ifndef Topology_H
#define Topology_H

#include <string>
#include <vector>
#include "RbDataType.h"

class Node;
class Topology : public RbDataType {

public:
	Topology(void);
	Topology(int n);
	Topology(Topology &t);
	Topology(Node* st); // copy of subtree
	~Topology(void); // return nodes to node factory?
	int getNumNodes(void) { return nodes.size(); }
	int getNumLeaves(void) {return leafNodes.size(); }
	int getNumInternalNodes(void) { return internalNodes.size(); }
	Node* getPostOrderNode(int i) { return postOrderSequence[i]; }
	Node* getRoot(void) { return root; }
	Node* getNode(int i) { return nodes[i]; }
	std::vector<Node*> &getNodeSet(void) { return nodes; }
	std::vector<Node*> &getLeafSet(void) { return leafNodes; }
	std::vector<Node*> &getInternalNodesSet(void) { return internalNodes; }
	
private:
	std::vector<Node*> nodes;
	std::vector<Node*> leafNodes;
	std::vector<Node*> internalNodes;
	std::vector<Node*> postOrderSequence;
	Node* root;
};

#endif