#ifndef Branch_H
#define Branch_H

#include <vector>

class Node;
class RbDataType;

class Branch {

public:
	Branch(void);
	Branch(Branch &b);
	// assignment operator
	// += operator

private:
	std::vector<RbDataType*> parameters;
	Node* parent;
	Node* child;
};

#endif