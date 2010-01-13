#ifndef NodeFactory_H
#define NodeFactory_H

#include <set>
#include <vector>

class Node;

class NodeFactory {

public:
	static NodeFactory& nodeFactoryInstance(void) 
		{
		static NodeFactory singleNodeFactory;
		return singleNodeFactory;
		}
	void drainNodePool(void);
	Node* getNode(void);
	void returnNodeToPool(Node* n);
	
private:
	NodeFactory(void) {};                        // private constructor, to prevent anyone but the class creating an instance
	NodeFactory(const NodeFactory&);             // prevent copy-construction
	NodeFactory& operator=(const NodeFactory&);  // prevent assignment
	~NodeFactory(void);
	std::vector<Node*> nodePool;
	std::set<Node*> allocatedNodes;
};

#endif

