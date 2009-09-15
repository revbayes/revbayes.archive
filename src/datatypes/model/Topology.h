/**
 * @file
 * This file contains the declaration of a topology object.
 * The topology is the structure of nodes building the tree.
 * In this case we are considering only rooted topologies.
 *
 * @brief Declaration of the class Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @extends RbDataType
 *
 * $Id$
 */
#ifndef Topology_H
#define Topology_H

#include <string>
#include <vector>
#include "../RbDataType.h"
#include "Node.h"

class Topology : public RbDataType {

public:
	Topology(void);                                                              //!< default constructor
	Topology(int n);                                                             //!< not sure anymore what this was for
	Topology(Topology &t);                                                       //!< copy constructor
	Topology(Node* r);                                                           //!< constructor with root as parameter
	~Topology(void);                                                             //!< return nodes to node factory?

	RbObject* clone();                                                           //!< deep copy of this topology

	int getNumberOfNodes(void);                                                  //!< get number of nodes in the tree
	int getNumberOfLeaves(void);                                                 //!< get number of leaf nodes
	int getNumberOFInternalNodes(void);                                          //!< get number of internal nodes
	Node* getPostOrderNode(int i);                                               //!< get node at position i in post-order traversal
	Node* getRoot(void);                                                         //!< get root node
	Node* getNode(int i);                                                        //!< get node at position i
	Node* getInternalNode(int i);                                                //!< get internal node at position i
	Node* getLeafNode(int i);                                                    //!< get leaf node at position i
	std::vector<Node*>& getNodeSet(void);                                        //!< get set of nodes
	std::vector<Node*>& getLeafSet(void);                                        //!< get set of leaf node
	std::vector<Node*>& getInternalNodesSet(void);                               //!< get set of internal nodes
	void addNode(Node* n);                                                       //!< add a node to the topology
	void addSubtree(Node* n);                                                    //!< add a whole subtree rooted at node n to the topology
	void setRoot(Node* n);                                                       //!< set the root of the tree
	void removeNode(Node* n);                                                    //!< remove a node to the topology
	void removeSubtree(Node* n);                                                 //!< remove a whole subtree rooted at node n to the topology
	
private:
	std::vector<Node*> nodes;                                                    //!< vector of pointers to all nodes of tree
	std::vector<Node*> leafNodes;                                                //!< vector of pointers to leaf nodes
	std::vector<Node*> internalNodes;                                            //!< vector of pointers to internal nodes
	std::vector<Node*> postOrderSequence;                                        //!< vector of pointers to nodes in post-order traversal of the tree
	Node* root;                                                                  //!< the root node of the tree
};

#endif
