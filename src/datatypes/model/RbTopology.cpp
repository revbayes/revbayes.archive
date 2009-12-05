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

#include "RbException.h"
#include "RbNames.h"
#include "RbNode.h"
#include "RbTopology.h"
#include "StringVector.h"

const StringVector RbTopology::rbClass = StringVector(RbNames::Topology::name) + RbComplex::rbClass;


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


RbTopology::RbTopology(const RbTopology& t) : RbComplex() {

    *base = *(t.base);
    fillNodeSets(base);
}

/**
 * @brief Destructor
 *
 * This is the destructor releasing the RbNodes to the RbNode factory and freeing the parameters.
 *
 */
RbTopology::~RbTopology(void) {

	for (std::set<RbNode*>::iterator p=nodes.begin(); p != nodes.end(); p++)
		delete (*p);
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
RbObject* RbTopology::clone(void) const {
	return new RbTopology(*this);
}

void RbTopology::fillNodeSets(RbNode* n) {

    const std::set<RbNode*>& children = n->getChildren();
    for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        fillNodeSets(*it);
    }

    if (n->isLeaf()) {
        leafNodes.insert(n);
    }
    else {
        internalNodes.insert(n);
    }
    nodes.insert(n);
    postOrderSequence.push_back(n);

}

void RbTopology::getDownpassSequence() {

    postOrderSequence.clear();
    getDownpassSequence(base);

}

void RbTopology::getDownpassSequence(RbNode* n) {

    const std::set<RbNode*>& children = n->getChildren();
    for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        fillNodeSets(*it);
    }
    postOrderSequence.push_back(n);

}

void RbTopology::getDownpassSequence(RbNode* n, std::vector<RbNode*>& seq) {

    const std::set<RbNode*>& children = n->getChildren();
    for (std::set<RbNode*>::iterator it=children.begin(); it!=children.end(); it++) {
        fillNodeSets(*it);
    }
    seq.push_back(n);

}

/**
 * @brief get number of RbNodes in the tree
 *
 * This function returns the number of RbNodes in the tree.
 *
 * @returns      the number of RbNodes in the tree
 *
 */
int RbTopology::getNumberOfNodes(void) {
	return nodes.size();
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
int RbTopology::getNumberOfInternalNodes(void) {
	return internalNodes.size();
}

/**
 * @brief get node at position i in post-order traversal
 *
 * This function returns the node at position i in post-order traversal of the tree.
 *
 * @param i          the position of the node
 * @returns          RbNode at position i of the post-order traversal
 *
 */
RbNode* RbTopology::getPostOrderNode(int i) {
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
RbNode* RbTopology::getBaseNode(void) {
	return base;
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
RbNode* RbTopology::getNode(int i) {
    int idx = 0;
    for (std::set<RbNode*>::iterator it=nodes.begin(); it!=nodes.begin(); it++) {
        if (idx == i) {
            return *it;
        }
        idx++;
    }

    return NULL;
}

/**
 * @brief get set of nodes
 *
 * This function returns the set of nodes.
 *
 * @returns      the set of nodes
 */
std::set<RbNode*>& RbTopology::getNodeSet(void) {
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
std::set<RbNode*>& RbTopology::getLeafSet(void) {
	return leafNodes;
}

/**
 * @brief get set of internal nodes
 *
 * This function returns the set of internal nodes.
 *
 * @returns      the set of internal nodes
 */
std::set<RbNode*>& RbTopology::getInternalNodesSet(void) {
	return internalNodes;
}

/**
 * @brief add a new node to the RbTopology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void RbTopology::addNode(RbNode* n){
	//check if it is a leaf RbNode
	if (n->isLeaf()){
		leafNodes.insert(n);
	}
	else {
		internalNodes.insert(n);
	}

	//add the RbNode to the set of all RbNodes
	nodes.insert(n);

	//recreate the post-order indexing
	//TODO
}

/**
 * @brief add a new node to the RbTopology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void RbTopology::addSubtree(RbNode* n){
	//check if it is a leaf node
	if (n->isLeaf()){
		leafNodes.insert(n);
	}
	else {
		internalNodes.insert(n);
		//add each child recursively
		for (std::set<RbNode*>::iterator it=n->getChildren().begin() ; it != n->getChildren().end(); it++ ){
			addSubtree(*it);
		}
	}

	//add the RbNode to the set of all RbNodes
	nodes.insert(n);

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
void RbTopology::setBaseNode(RbNode* n){
	base = n;

	//TODO need to consider if we want to recreate here the whole tree automatically
}

bool RbTopology::isBaseNode(RbNode* n){
    base == n;

}

/**
 * @brief add a new node to the RbTopology
 *
 * This function adds a new node to the set.
 *
 * @param        the node to add to the sets
 */
void RbTopology::removeNode(RbNode* n){
	//check if it is a leaf node
		if (n->isLeaf()){
			// remove n
			nodes.erase(n);
		}
		else {
			//remove each child recursively
			for (std::set<RbNode*>::iterator it=n->getChildren().begin() ; it != n->getChildren().end(); it++ ){
				removeSubtree(*it);
			}
			leafNodes.erase(n);
		}

		//remove the node from the set of all nodes
		nodes.erase(n);

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
	    leafNodes.erase(n);
	}
	else {
		//remove each child recursively
		for (std::set<RbNode*>::iterator it=n->getChildren().begin() ; it != n->getChildren().end(); it++ ){
			removeSubtree(*it);
		}

		// remove n
		internalNodes.erase(n);
	}

	nodes.erase(n);

	//recreate the post-order indexing
	//TODO
}

bool RbTopology::equals(const RbObject* obj) const {
    return (this == obj);
}

const StringVector& RbTopology::getClass(void) const {
    rbClass;
}

void RbTopology::print(std::ostream& o) const {
    o << "Tree Topology" << std::endl;
}

void RbTopology::printValue(std::ostream& o) const {
//  o << "Unrooted Tree" << std::endl;
}

std::string RbTopology::toString(void) const {
    std::string tmp = "Tree Topology";
    return tmp;
}

RbObject& RbTopology::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const RbTopology& x = dynamic_cast<const RbTopology&> (obj);

        RbTopology& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const RbTopology& x = dynamic_cast<const RbTopology&> (*(obj.convertTo(RbNames::Topology::name)));

            RbTopology& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to " + RbNames::Topology::name);
            throw e;
        }
    }

    // dummy return
    return (*this);
}

RbTopology& RbTopology::operator=(const RbTopology& obj) {

    nodes.clear();
    internalNodes.clear();
    leafNodes.clear();
    postOrderSequence.clear();

    *base = *(obj.base);
    fillNodeSets(base);

    return (*this);
}
