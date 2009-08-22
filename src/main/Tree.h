#ifndef Tree_H
#define Tree_H

#include <string>
#include <vector>
#include "RbDataType.h"

class DAGNode;
class Topology;
class Tree : public RbDataType {

public:
	Tree(void);
	Tree(Tree &t);
	~Tree(void); // return nodes to node factory?
	void associateBranchParms(std::vector<DAGNode*> &brns);
	
private:
	DAGNode* topology;
	std::vector<DAGNode*> branchParms;
	std::vector<DAGNode*> nodeParms;
};

#endif