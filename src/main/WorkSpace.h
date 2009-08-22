#ifndef WorkSpace_H
#define WorkSpace_H

#include <map>
#include <set>
#include <string>

class DAGNode;
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
};

#endif
