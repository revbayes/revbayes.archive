#ifndef Tree_H
#define Tree_H

#include <string>
#include <vector>
#include <map>
#include "../RbDataType.h"
#include "Node.h"
#include "Branch.h"
#include "Topology.h"

class Tree : public RbDataType {

public:
	Tree(void);
	Tree(Tree &t);
	~Tree(void); // return nodes to node factory?
	
	void addNodeParameter(Node* n, RbDataType* p);
	void addNodeParameter(Node* n, std::vector<RbDataType*> p);             // add a parameter to the node

	void assignNodeToBranch(Node* n, Branch* b);                            // assign a new branch b to the node n
	void assignNodeToBranch(Node* n, std::vector<Branch*> b);               // assign the set b of new branches to the node n

private:
	Topology* topology;
	std::vector<Branch*> branch;
	std::map<Node*,std::vector<Branch*>> nodeBranchAssignment;      // the association between nodes and branches
//	std::vector<RbDataType*> branchParms;
//	std::vector<RbDataType*> nodeParms;
};

#endif
