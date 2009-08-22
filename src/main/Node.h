#ifndef Node_H
#define Node_H

#include <string>
#include <vector>
#include "RbDataType.h"

class Node {

public:
	Node(void);
	Node* getChild(int i) { return children[i]; }
	std::vector<Node*> &getChildren(void) { return children; }
	Node* getParent(void) { return parent; }
	void setName(std::string s) { name = s; }
	std::string getName(void) { return name; }
	int getNumChildren(void) { return children.size(); }
	void clearChildren(void) { children.clear(); }
	void clean(void);
	int getIndex(void) { return index; }
	void setIndex(int i) { index = i; }

private:
	std::vector<Node*> children;
	Node* parent;
	std::string name;
	int index;
};

#endif