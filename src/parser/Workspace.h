#ifndef WorkSpace_H
#define WorkSpace_H

#include <map>
#include <set>
#include <string>

#include "../dag/DAGNode.h"
#include "../moves/Move.h"
#include "../main/Monitor.h"

class WorkSpace {

public:
	static WorkSpace& workSpaceInstance(void)
		{
		static WorkSpace singleWorkSpace;
		return singleWorkSpace;
		}
	void clean(void);
	DAGNode* retrieveDagNode(std::string &q);
	void addDagNode(DAGNode *dn);
	
private:
	WorkSpace(void) {};
	WorkSpace(const WorkSpace&);
	~WorkSpace(void);
	WorkSpace& operator=(const WorkSpace&);
	std::set<DAGNode*> dagNodes;
	std::map<std::string,DAGNode*> symbolTable;
	std::map<Move*,DAGNode*> movesTable;
	std::map<Monitor*,DAGNode*> monitorTable;
};

#endif
