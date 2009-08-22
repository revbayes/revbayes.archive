#ifndef Move_H
#define Move_H

#include <set>
#include "RbObject.h"

class DAGNode;
class Model : public RbObject {

public:

	Model(void);
	Model(std::set<DAGNode*> &sn);
	Model(Model &m);
	bool completeModel(void);

private:
	DAGNode* sinkNodes;
};

#endif