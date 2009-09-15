/**
 * @file
 * This file contains the implementation of a topology object.
 * The topology is the structure of nodes building the tree.
 * In this case we are considering only rooted topologies.
 *
 * @brief Implementation of the class Topology
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

#include "Node.h"
#include "../../factory/NodeFactory.h"
#include "Topology.h"

/**
 * @brief default constructor
 *
 * This is the default constructor.
 *
 */
Topology::Topology(void) : RbDataType("Topology") {

}

/**
 * @brief brief description
 *
 * This function does foo.
 *
 * @param param      input param
 * @pre              the precondition of an entity
 * @see function     another similar function, e.g. overloaded one, called one, ...
 * @returns return value
 * @throw        may throw this exception
 */
Topology::Topology(int n) : RbDataType("Topology"){
	//TODO need to check use of it, i.e. why we have and what we want it for
}

/**
 * @brief copy constructor
 *
 * This is the copy constructor.
 *
 * @param t          the topology to copy
 *
 */
Topology::Topology(Topology &t) : RbDataType("Topology") {
	//TODO need to implement this
	// possibly John's way of a deep copy wold be good
}

/**
 * @brief constructor
 *
 * This is the constructor with root as parameter.
 *
 * @param r          the root node
 *
 */
Topology::Topology(Node* r) : RbDataType("Topology") {
	// set the root
	root = r;

	//TODO
	// set the vector with all node, leaf nodes, internal nodes, post-order sorting

}

/**
 * @brief Destructor
 *
 * This is the destructor releasing the nodes to the node factory and freeing the parameters.
 *
 */
Topology::~Topology(void) {

	NodeFactory &f = NodeFactory::nodeFactoryInstance();
	for (std::vector<Node*>::iterator p=nodes.begin(); p != nodes.end(); p++)
		f.returnNodeToPool( *p );
}

/**
 * @brief clone this topology
 *
 * This function creates a deep copy of itself. It calls the copy constructor for doing this.
 * This function is needed to create deep copy called when references only to a base object are available and the derivation is unknown.
 *
 * @returns      clone of the topology
 *
 */
RbObject* Topology::clone(void) {
	return new Topology(this);
}

/**
 * @brief get number of nodes in the tree
 *
 * This function returns the number of nodes in the tree.
 *
 * @returns      the number of nodes in the tree
 *
 */
int Topology::getNumberOfNodes(void) {
	return nodes.size();
}

/**
 * @brief get number of leaf nodes
 *
 * This function return the number of leaf nodes.
 *
 * @returns      the number of leaf nodes
 *
 */
int Topology::getNumberOfLeaves(void) {
	return leafNodes.size();
}

/**
 * @brief get number of internal nodes
 *
 * This function returns the number of internal nodes of the tree.
 *
 * @returns      the number of internal nodes
 *
 */
int Topology::getNumberOfInternalNodes(void) {
	return internalNodes.size();
}

/**
 * @brief get node at position i in post-order traversal
 *
 * This function returns the node at position i in post-order traversal of the tree.
 *
 * @param i          the position of the node
 * @returns          node at position i of the post-order traversal
 *
 */
Node* Topology::getPostOrderNode(int i) {
	return postOrderSequence[i];
}

/**
 * @brief get root node
 *
 * This function returns the root node.
 *
 * @returns      the root node of the tree
 *
 */
Node* Topology::getRoot(void) {
	return root;
}

/**
 * @brief get node at position i
 *
 * This function returns the node at position i. The ordering can be arbitrary but remains constant as long as the nodes are not changed.
 *
 * @param i          the position of the node
 * @returns          the node at position i
 *
 */
Node* Topology::getNode(int i) {
	return nodes[i];
}

/**
 * @brief get set of nodes
 *
 * This function returns the set of nodes.
 *
 * @returns      the set of nodes
 */
std::vector<Node*>& Topology::getNodeSet(void) {
	return nodes;
}

/**
 * @brief get set of leaf nodes
 *
 * This function the set of leaf nodes.
 *
 * @returns      the set of leaf nodes
 *
 */
std::vector<Node*>& Topology::getLeafSet(void) {
	return leafNodes;
}

/**
 * @brief get set of internal nodes
 *
 * This function returns the set of internal nodes.
 *
 * @returns      the set of internal nodes
 */
std::vector<Node*>& Topology::getInternalNodesSet(void) {
	return internalNodes;
}

/**
 * @brief add a new node to the topology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void Topology::addNode(Node* n){
	//check if it is a leaf node
	if (n->isLeaf()){
		leafNodes.push_back(n);
	}
	else {
		internalNodes.push_back(n);
	}

	//add the node to the set of all nodes
	nodes.push_back(n);

	//recreate the post-order indexing
	//TODO
}

/**
 * @brief add a new node to the topology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void Topology::addSubtree(Node* n){
	//check if it is a leaf node
	if (n->isLeaf()){
		leafNodes.push_back(n);
	}
	else {
		internalNodes.push_back(n);
		//add each child recursively
		for (std::vector<Node*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
			addSubtree(*it);
		}
	}

	//add the node to the set of all nodes
	nodes.push_back(n);

	//recreate the post-order indexing
	//TODO
}

/**
 * @brief add a new node to the topology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void Topology::setRoot(Node* n){
	root = n;

	//TODO need to consider if we want to recreate here the whole tree automatically
}

/**
 * @brief add a new node to the topology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void Topology::removeNode(Node* n){
	//check if it is a leaf node
		if (n->isLeaf()){
			// remove n
			for (std::vector<RbDataType*>::iterator it=leafNodes.begin() ; it < leafNodes.end(); it++ ){
				if (*it == n){
					leafNodes.erase(it);
					break;
				}
			}
		}
		else {
			//remove each child recursively
			for (std::vector<Node*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
				removeSubtree(*it);
			}
		}

		//remove the node to the set of all nodes
		for (std::vector<RbDataType*>::iterator it=nodes.begin() ; it < nodes.end(); it++ ){
			if (*it == n){
				nodes.erase(it);
				break;
			}
		}

		//recreate the post-order indexing
		//TODO
}

/**
 * @brief add a new node to the topology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void Topology::removeSubtree(Node* n){
	//check if it is a leaf node
	if (n->isLeaf()){
		// remove n
		for (std::vector<RbDataType*>::iterator it=leafNodes.begin() ; it < leafNodes.end(); it++ ){
			if (*it == n){
				leafNodes.erase(it);
				break;
			}
		}
	}
	else {
		//remove each child recursively
		for (std::vector<Node*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
			removeSubtree(*it);
		}

		// remove n
		for (std::vector<RbDataType*>::iterator it=internalNodes.begin() ; it < internalNodes.end(); it++ ){
			if (*it == n){
				internalNodes.erase(it);
				break;
			}
		}
	}

	//remove the node to the set of all nodes
	for (std::vector<RbDataType*>::iterator it=nodes.begin() ; it < nodes.end(); it++ ){
		if (*it == n){
			nodes.erase(it);
			break;
		}
	}

	//recreate the post-order indexing
	//TODO
}
