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


/**
 * Copy constructor should not copy children because it creates an
 * independent node. Name also needs to be reset for the same reason.
 * The new node is simply not used by any other objects at this point.
 * The parent nodes are left empty here because they are better dealt
 * with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode(const DAGNode& x)
    : children(), parents(), name(""), valueType(x.valueType) {
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
void DAGNode::setElement(const IntVector& index, DAGNode* var) {

    throw (RbException("No variable elements"));
}


