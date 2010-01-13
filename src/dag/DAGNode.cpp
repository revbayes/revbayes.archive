/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG.
 *
 * @brief Implementation of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:52:44 +0100 (Tis, 12 Jan 2010) $
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface Distribution
 * @extends RbObject
 *
 * $Id: DAGNode.cpp 222 2010-01-12 20:52:44Z Hoehna $
 */

#include "DAGNode.h"
#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "StringVector.h"
#include "VariableNode.h"

#include <algorithm>
#include <iostream>


/** Constructor: set value type */
DAGNode::DAGNode(const std::string& valType)
    : children(), parents(), name(""), valueType(valType) {
}


/** Destructor needs to be careful with graph and references */
DAGNode::~DAGNode() {

    if (children.size() != 0)
        throw RbException("Invalid deletion: node with children");

    /* Remove connections and delete orphan nodes */
    for(std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
        (*i)->removeChildNode((VariableNode*)(this));
        if ((*i)->numRefs() == 0)
            delete (*i);
    }
}


/** Clone the entire graph */
DAGNode* DAGNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    DAGNode* temp = clone();

    temp->children.clear();
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        if (newNodes.find((DAGNode*)(*i)) == newNodes.end())
            newNodes[(DAGNode*)(*i)] = (*i)->cloneDAG(newNodes);
        temp->children.insert((VariableNode*)(newNodes[(DAGNode*)(*i)]));
    }

    temp->parents.clear();
    for(std::set<DAGNode*>::const_iterator i=parents.begin(); i!=parents.end(); i++) {
        if (newNodes.find(*i) == newNodes.end())
            newNodes[(*i)] = (*i)->cloneDAG(newNodes);
        temp->parents.insert(newNodes[(*i)]);
    }

    return temp;
}


/** Get class vector describing type of object */
const StringVector& DAGNode::getClass() const {

    static StringVector rbClass = StringVector(DAGNode_name);
    return rbClass;
}


/** Get type of wrapper (first entry in class vector) */
const std::string& DAGNode::getType(void) const { 

    return getClass()[0];
}


/** Get element variable; default throws error, override if wrapper has variable elements */
const DAGNode* DAGNode::getVarElement(const IntVector& index) const {

    throw (RbException("No variable elements"));
}


/** Is wrapper of specified type? We need to check entire class vector in case we are derived from type. */
bool DAGNode::isType(const std::string& type) const {

    const StringVector& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

    return false;
}


/** Check if node is a parent of node x in the DAG: needed to check for cycles in the DAG */
bool DAGNode::isParentInDAG(const DAGNode* x, std::list<DAGNode*>& done) const {

    for(std::set<DAGNode*>::const_iterator i=parents.begin(); i!=parents.end(); i++) {
        if (find(done.begin(), done.end(), (*i)) == done.end()) {
            if ((*i)->isParentInDAG(x, done))
                return true;
        }
    }

    return false;
}


/** Get number of references to the node from Frame and other DAG nodes */
int DAGNode::numRefs(void) const {

    if (name == "")
        return numChildren();
    else
        return numChildren() + 1;
}


/** Print children */
void DAGNode::printChildren(std::ostream& o) const {

    if ( children.empty() ) {
        o << "No children" << std::endl;
        return;
    }

    int count = 1;
    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++, count++) {
        o << "children[" << count << "] = <DAG ptr> " << (*i) << " of type " << (*i)->getType() << std::endl;
    }
}


/** Print parents */
void DAGNode::printParents(std::ostream& o) const {

    if ( parents.empty() ) {
        o << "No parents" << std::endl;
        return;
    }

    int count = 1;
    for (std::set<DAGNode*>::const_iterator i=parents.begin(); i != parents.end(); i++, count++) {
        o << "parents[" << count << "] = <DAG ptr> " << (*i) << " of type " << (*i)->getType() << std::endl;
    }
}


/** Set element variable; default throws error, override if wrapper has variable elements */
void DAGNode::setElement(const IntVector& index, DAGNode* var) {

    throw (RbException("No variable elements"));
}


