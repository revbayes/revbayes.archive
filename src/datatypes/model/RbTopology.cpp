/**
 * @file
 * This file contains the implementation of a topology object.
 * The topology is the structure of nodes building the tree.
 * In this case we are considering only rooted topologies.
 *
 * @brief Implementation of the class Topology
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-09-15 08:28:53 -0700 (Tue, 15 Sep 2009) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-21, version 1.0
 * @extends RbDataType
 *
 * $Id: Topology.cpp 36 2009-09-15 15:28:53Z Hoehna $
 */

#include "RbNode.h"
#include "RbNodeFactory.h"
#include "RbTopology.h"

/**
 * @brief default constructor
 *
 * This is the default constructor.
 *
 */
RbTopology::RbTopology(void) : RbComplex() {

}

/**
 * @brief default constructor
 *
 * This is the default constructor.
 *
 */
RbTopology::RbTopology(RbNode* bn) : RbComplex() {
	base = bn;
	fillNodeSets(bn);
}


/**
 * @brief Destructor
 *
 * This is the destructor releasing the RbNodes to the RbNode factory and freeing the parameters.
 *
 */
RbTopology::~RbTopology(void) {

	RbNodeFactory &f = RbNodeFactory::RbNodeFactoryInstance();
	for (std::vector<RbNode*>::iterator p=RbNodes.begin(); p != RbNodes.end(); p++)
		f.returnRbNodeToPool( *p );
}

/**
 * @brief clone this RbTopology
 *
 * This function creates a deep copy of itself. It calls the copy constructor for doing this.
 * This function is needed to create deep copy called when references only to a base object are available and the derivation is unknown.
 *
 * @returns      clone of the RbTopology
 *
 */
RbObject* RbTopology::clone(void) {
	return new RbTopology(this);
}

void RbTopology::fillNodeSets() {

}

/**
 * @brief get number of RbNodes in the tree
 *
 * This function returns the number of RbNodes in the tree.
 *
 * @returns      the number of RbNodes in the tree
 *
 */
int RbTopology::getNumberOfRbNodes(void) {
	return RbNodes.size();
}

/**
 * @brief get number of leaf RbNodes
 *
 * This function return the number of leaf RbNodes.
 *
 * @returns      the number of leaf RbNodes
 *
 */
int RbTopology::getNumberOfLeaves(void) {
	return leafRbNodes.size();
}

/**
 * @brief get number of internal RbNodes
 *
 * This function returns the number of internal RbNodes of the tree.
 *
 * @returns      the number of internal RbNodes
 *
 */
int RbTopology::getNumberOfInternalRbNodes(void) {
	return internalRbNodes.size();
}

/**
 * @brief get RbNode at position i in post-order traversal
 *
 * This function returns the RbNode at position i in post-order traversal of the tree.
 *
 * @param i          the position of the RbNode
 * @returns          RbNode at position i of the post-order traversal
 *
 */
RbNode* RbTopology::getPostOrderRbNode(int i) {
	return postOrderSequence[i];
}

/**
 * @brief get root RbNode
 *
 * This function returns the root RbNode.
 *
 * @returns      the root RbNode of the tree
 *
 */
RbNode* RbTopology::getRoot(void) {
	return root;
}

/**
 * @brief get RbNode at position i
 *
 * This function returns the RbNode at position i. The ordering can be arbitrary but remains constant as long as the RbNodes are not changed.
 *
 * @param i          the position of the RbNode
 * @returns          the RbNode at position i
 *
 */
RbNode* RbTopology::getRbNode(int i) {
	return RbNodes[i];
}

/**
 * @brief get set of RbNodes
 *
 * This function returns the set of RbNodes.
 *
 * @returns      the set of RbNodes
 */
std::vector<RbNode*>& RbTopology::getRbNodeSet(void) {
	return RbNodes;
}

/**
 * @brief get set of leaf RbNodes
 *
 * This function the set of leaf RbNodes.
 *
 * @returns      the set of leaf RbNodes
 *
 */
std::vector<RbNode*>& RbTopology::getLeafSet(void) {
	return leafRbNodes;
}

/**
 * @brief get set of internal RbNodes
 *
 * This function returns the set of internal RbNodes.
 *
 * @returns      the set of internal RbNodes
 */
std::vector<RbNode*>& RbTopology::getInternalRbNodesSet(void) {
	return internalRbNodes;
}

/**
 * @brief add a new RbNode to the RbTopology
 *
 * This function adds a new RbNode to the set.
 *
 * @param        the RbNode to add to the sets
 */
void RbTopology::addRbNode(RbNode* n){
	//check if it is a leaf RbNode
	if (n->isLeaf()){
		leafRbNodes.push_back(n);
	}
	else {
		internalRbNodes.push_back(n);
	}

	//add the RbNode to the set of all RbNodes
	RbNodes.push_back(n);

	//recreate the post-order indexing
	//TODO
}

/**
 * @brief add a new RbNode to the RbTopology
 *
 * This function adds a new RbNode to the set.
 *
 * @param        the RbNode to add to the sets
 */
void RbTopology::addSubtree(RbNode* n){
	//check if it is a leaf RbNode
	if (n->isLeaf()){
		leafRbNodes.push_back(n);
	}
	else {
		internalRbNodes.push_back(n);
		//add each child recursively
		for (std::vector<RbNode*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
			addSubtree(*it);
		}
	}

	//add the RbNode to the set of all RbNodes
	RbNodes.push_back(n);

	//recreate the post-order indexing
	//TODO
}

/**
 * @brief add a new RbNode to the RbTopology
 *
 * This function adds a new RbNode to the set.
 *
 * @param        the RbNode to add to the sets
 */
void RbTopology::setRoot(RbNode* n){
	root = n;

	//TODO need to consider if we want to recreate here the whole tree automatically
}

/**
 * @brief add a new RbNode to the RbTopology
 *
 * This function adds a new RbNode to the set.
 *
 * @param        the RbNode to add to the sets
 */
void RbTopology::removeRbNode(RbNode* n){
	//check if it is a leaf RbNode
		if (n->isLeaf()){
			// remove n
			for (std::vector<RbNode*>::iterator it=leafRbNodes.begin() ; it < leafRbNodes.end(); it++ ){
				if (*it == n){
					leafRbNodes.erase(it);
					break;
				}
			}
		}
		else {
			//remove each child recursively
			for (std::vector<RbNode*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
				removeSubtree(*it);
			}
		}

		//remove the RbNode to the set of all RbNodes
		for (std::vector<RbNode*>::iterator it=RbNodes.begin() ; it < RbNodes.end(); it++ ){
			if (*it == n){
				RbNodes.erase(it);
				break;
			}
		}

		//recreate the post-order indexing
		//TODO
}

/**
 * @brief add a new RbNode to the RbTopology
 *
 * This function adds a new RbNode to the set.
 *
 * @param        the RbNode to add to the sets
 */
void RbTopology::removeSubtree(RbNode* n){
	//check if it is a leaf RbNode
	if (n->isLeaf()){
		// remove n
		for (std::vector<RbNode*>::iterator it=leafRbNodes.begin() ; it < leafRbNodes.end(); it++ ){
			if (*it == n){
				leafRbNodes.erase(it);
				break;
			}
		}
	}
	else {
		//remove each child recursively
		for (std::vector<RbNode*>::iterator it=n->getChildren().begin() ; it < n->getChildren().end(); it++ ){
			removeSubtree(*it);
		}

		// remove n
		for (std::vector<RbNode*>::iterator it=internalRbNodes.begin() ; it < internalRbNodes.end(); it++ ){
			if (*it == n){
				internalRbNodes.erase(it);
				break;
			}
		}
	}

	//remove the RbNode to the set of all RbNodes
	for (std::vector<RbNode*>::iterator it=RbNodes.begin() ; it < RbNodes.end(); it++ ){
		if (*it == n){
			RbNodes.erase(it);
			break;
		}
	}

	//recreate the post-order indexing
	//TODO
}
