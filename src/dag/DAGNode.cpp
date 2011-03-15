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
#include "Frame.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>
#include <iostream>


/** Constructor: set value type */
DAGNode::DAGNode(const std::string& valType) : children(), parents(), referringFrames(), name(""), valueType(valType) {

}

/**
 * Copy constructor should not copy children because it creates an
 * independent node. Name also needs to be reset for the same reason.
 * The new node is simply not used by any other objects at this point.
 * The parent nodes are left empty here because they are better dealt
 * with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode(const DAGNode& x) : children(), parents(), referringFrames(), name(""), valueType(x.valueType) {

}

/** Get class vector describing type of object */
const VectorString& DAGNode::getClass() const {

    static VectorString rbClass = VectorString(DAGNode_name);
    return rbClass;
}

/** Get name of DAG node from its surrounding objects */
std::string DAGNode::getName(void) const {

    std::string name;

    if (slot == NULL) {
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            if ((*i)->isType(DAGNodePlate_name)) {
                DAGNodePlate* thePlate = (DAGNodePlate*)(*i);
                name = thePlate->getName();
                VectorInteger index = thePlate->getIndex(*i);
                for (size_t j=0; j<index.size(); j++)
                    name += "[" + std::string(index[j] + 1) + "]";
            }
        }
    }
    else {
        name = slot->getName();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            if ((*i)->isType(CompositeNode_name)) {
                name = (*i)->getName() + "." + name;
            }        
        }
    }
    return name;
}

/** Get type of wrapper (first entry in class vector) */
const std::string& DAGNode::getType(void) const { 

    return getClass()[0];
}

/** Get element variable; default throws error, override if wrapper has variable elements */
const DAGNode* DAGNode::getVarElement(const VectorInteger& index) const {

    throw (RbException("No variable elements"));
}

/** Is wrapper of specified type? We need to check entire class vector in case we are derived from type. */
bool DAGNode::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

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

/** Get number of references to the node from Frame and other DAG nodes
 *  This code relies on name being set if the node is owned by a frame */
int DAGNode::numRefs(void) const {

    if (frame == NULL)
        return numChildren() + referringFrames.size();
    else
        return numChildren() + referringFrames.size() + 1;
}

/** Print children */
void DAGNode::printChildren(std::ostream& o) const {

    if ( children.empty() ) {
        o << "No children" << std::endl;
        return;
    }

    int count = 1;
    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++, count++) {
        o << "children[" << count << "] = '" << (*i)->getName();
        o << "' [" << (*i) << "] of type " << (*i)->getType() << std::endl;
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
        o << "parents[" << count << "] = '" << (*i)->getName();
        o << "' [" << (*i) << "] of type " << (*i)->getType() << std::endl;
    }
}


/** Set element variable; default throws error, override if wrapper has variable elements */
void DAGNode::setElement(const VectorInteger& index, DAGNode* var) {

    throw (RbException("No variable elements"));
}


/** Swap node in DAG to new node */
void DAGNode::swapNodeTo(DAGNode* newNode) {
    
    // It is important to have a copy here as children will be changed in process
    std::set<VariableNode*> oldChildren = getChildren();
    for (std::set<VariableNode*>::iterator i=oldChildren.begin(); i!=oldChildren.end(); i++)
            (*i)->swapParentNode(this, newNode);

    // Update referring frames
    for (std::set<Frame*>::iterator i=referringFrames.begin(); i!=referringFrames.end(); i++)
        (*i)->swapReference(this, newNode);
}

