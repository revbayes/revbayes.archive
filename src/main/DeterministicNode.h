#ifndef DeterministicNode_H
#define DeterministicNode_H

#include <string>
#include <vector>
#include "DAGNode.h"

class RbDataType;
class SyntaxElement;
class DeterministicNode : public DAGNode {

public:
	DeterministicNode(RbDataType* dt);
	bool isChanged(void) { return true; }
	void print(void);

private:
	SyntaxElement* expression;
};

#endif