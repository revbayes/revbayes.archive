/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG.
 *
 * @brief Implementation of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:52:44 +0100 (Tis, 12 Jan 2010) $
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface Distribution
 * @extends RbObject
 *
 * $Id: DAGNode.cpp 222 2010-01-12 20:52:44Z Hoehna $
 */

#include "DAGNode.h"
#include "ContainerNode.h"
#include "Frame.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbString.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>
#include <iostream>


/** Constructor: set value type */
DAGNode::DAGNode(const std::string& valType) : children(), parents(), slot(), referringSlots(), valueType(valType) {
}


/**
 * Copy constructor should not copy children because it creates an
 * independent node. The new node is simply not used by any other objects
 * at this point. The parent nodes are left empty here because they are better
 * dealt with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode(const DAGNode& x) : children(), parents(), slot(), referringSlots(), valueType(x.valueType) {

}


/** Get class vector describing type of DAG node */
const VectorString& DAGNode::getDAGClass() const {

    static VectorString rbClass = VectorString(DAGNode_name);
    return rbClass;
}


/** Get name of DAG node from its surrounding objects */
const std::string DAGNode::getName(void) const {

    std::string name;

    if (slot == NULL) {
        for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
            if ((*i)->isDAGType(ContainerNode_name)) {
                ContainerNode* theContainer = (ContainerNode*)(*i);
                name = theContainer->getName();
                VectorInteger index = theContainer->getIndex(*i);
                for (size_t j=0; j<index.size(); j++)
                    name += "[" + RbString(index[j] + 1) + "]";
                break;
            }
        }
    }
    else {
        name = slot->getName();
    }

    return name;
}


/** Get type of DAG node (first entry in class vector) */
const std::string& DAGNode::getDAGType(void) const { 

    return getDAGClass()[0];
}


/** Is DAG node of specified type? We need to check entire class vector in case we are derived from type. */
bool DAGNode::isDAGType(const std::string& type) const {

    const VectorString& classVec = getDAGClass();

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

    if (slot == NULL)
        return numChildren() + referringSlots.size();
    else
        return numChildren() + referringSlots.size() + 1;
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
        o << "' [" << (*i) << "] of type " << (*i)->getDAGType() << std::endl;
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
        o << "' [" << (*i) << "] of type " << (*i)->getDAGType() << std::endl;
    }
}


/** Remove a slot referring to the DAG node */
void DAGNode::removeSlot(const VariableSlot* s) {

    if ( slot == s ) {
        if ( referringSlots.size() == 0 )
            slot = NULL;
        else {
            slot = *referringSlots.begin();
            referringSlots.erase( referringSlots.begin() );
        }
    }
    else {
        std::set<VariableSlot*>::iterator i = std::find( referringSlots.begin(), referringSlots.end(), s );
        if (i == referringSlots.end() )
            throw RbException( "Variable '" + getName() + " not aware of the reference from slot '" + s->getName() + "'" );
        referringSlots.erase( i );
    }
}


/** Swap node in DAG to new node */
void DAGNode::swapNodeTo(DAGNode* newNode) {
    
    // It is important to have a copy here as children will be changed in process
    std::set<VariableNode*> oldChildren = getChildren();
    for (std::set<VariableNode*>::iterator i=oldChildren.begin(); i!=oldChildren.end(); i++)
            (*i)->swapParentNode(this, newNode);

    // Update referring frames
    for (std::set<VariableSlot*>::iterator i=referringSlots.begin(); i!=referringSlots.end(); i++)
        (*i)->setReference(newNode);
}

