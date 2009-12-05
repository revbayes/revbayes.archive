/**
 * @file
 * This file contains the declaration of a topology object.
 * The topology itself is only a container of nodes. The structure of the tree is given by the nodes.
 * In this case we are considering only rooted topologies. Unrooted topologies must define a root, which might be any internal node.
 * Note, the root should not have any parent and is therefore treated different than other notes.
 *
 * @brief Declaration of the class Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-09-22 09:17:28 -0700 (Tue, 22 Sep 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @extends RbDataType
 *
 * $Id: Topology.h 42 2009-09-22 16:17:28Z Hoehna $
 */
#ifndef Topology_H
#define Topology_H

#include <string>
#include <set>
#include <vector>
#include "RbComplex.h"
#include "RbNode.h"

class RbTopology : public RbComplex {

public:

	virtual ~RbTopology(void);                                                             //!< return RbNodes to RbNode factory?
//	RbObject* clone();                                                           //!< deep copy of this topology

	int getNumberOfNodes(void);                                                  //!< get number of nodes in the tree
	int getNumberOfLeaves(void);                                                 //!< get number of leaf nodes
	int getNumberOFInternalNodes(void);                                          //!< get number of internal nodes
	RbNode* getPostOrderNode(int i);                                               //!< get node at position i in post-order traversal
	RbNode* getBaseNode(void);                                                         //!< get root node
	RbNode* getNode(int i);                                                        //!< get node at position i
	RbNode* getInternalNode(int i);                                                //!< get internal node at position i
	RbNode* getLeafNode(int i);                                                    //!< get leaf node at position i
	std::vector<RbNode*>& getNodeSet(void);                                        //!< get set of nodes
	std::vector<RbNode*>& getLeafSet(void);                                        //!< get set of leaf node
	std::vector<RbNode*>& getInternalNodesSet(void);                               //!< get set of internal nodes
	bool isBaseNode(RbNode* n);
	void addNode(RbNode* n);                                                       //!< add a node to the topology
	void addSubtree(RbNode* n);                                                    //!< add a whole subtree rooted at node n to the topology
	void setBaseNode(RbNode* n);                                                       //!< set the root of the tree
	void removeNode(RbNode* n);                                                    //!< remove a node to the topology
	void removeSubtree(RbNode* n);                                                 //!< remove a whole subtree rooted at node n to the topology
	
protected:
	RbTopology(void);                                                              //!< default constructor
	RbTopology(RbNode* bn);
//	RbTopology(int n);                                                             //!< not sure anymore what this was for
//	RbTopology(RbTopology &t);                                                       //!< copy constructor
//	RbTopology(RbNode* r);                                                           //!< constructor with root as parameter

	std::set<RbNode*> nodes;                                                    //!< vector of pointers to all nodes of tree
	std::set<RbNode*> leafNodes;                                                //!< vector of pointers to leaf nodes
	std::set<RbNode*> internalNodes;                                            //!< vector of pointers to internal nodes
	std::vector<RbNode*> postOrderSequence;                                        //!< vector of pointers to nodes in post-order traversal of the tree
	RbNode* base;                                                                  //!< the root node of the tree
	
	private:
	void fillNodeSets(void);
};

#endif
