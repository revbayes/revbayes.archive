#ifndef StochasticNode_H
#define StochasticNode_H

#include <string>
#include <vector>
#include "DAGNode.h"

class Distribution;
class RbDataType;
class StochasticNode : public DAGNode {

public:
	StochasticNode(void);
	StochasticNode(RbDataType* dat, Distribution *dist);
	~StochasticNode(void);
	Distribution* getDistribution(void) { return distribution; }
	bool isChanged(void) { return true; }
	void print(void);

private:
	Distribution* distribution;
};

#endif