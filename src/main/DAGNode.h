#ifndef DAGNode_H
#define DAGNode_H

#include <string>
#include <set>
#include "RbDataType.h"
#include "RbObject.h"

class DAGNode : public RbObject {

public:
	    DAGNode(RbDataType *dt);
	    //DAGNode(RbDataType *dt, std::set<DAGNode*> &p, std::set<DAGNode*> &c);
	    DAGNode(DAGNode &d);
	    ~DAGNode(void);

	virtual bool        isChanged(void)=0;
	virtual void        print(void)=0;
	std::set<DAGNode*>& getChildrenNodes(void) { return children; }
	std::set<DAGNode*>& getParentNodes(void) { return parents; }
	void                addParentNode(DAGNode* p) { parents.insert(p); }
	//void              addParentNode(std::set<DAGNode*> p); REMEMBER TO OVERLOAD THESE ADD/REMOVEs WITH SETS
	void                removeParentNode(DAGNode* p) { parents.erase(p); }
	void                addChildNode(DAGNode* c) { children.insert(c); }
	void                removeChildNode(DAGNode* c) { children.erase(c); }
	void                printChildren(void);
	void                printParents(void);
	
protected:
	RbDataType*         value;
	RbDataType*         storedValue;
	std::set<DAGNode*>  parents;
	std::set<DAGNode*>  children;
};

#endif
