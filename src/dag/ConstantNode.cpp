/**
 * @file
 * This file contains the implementation of ConstantNode, which is derived
 * from ConstantNode. ConstantNode is used for DAG nodes holding constant
 * values, and generally for variables used in RevBayes.
 *
 * @brief Declaration of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends ConstantNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */


#include "ConstantNode.h"
#include <iostream>
#include "DAGNode.h"
#include <string>
#include <set>
#include "RbException.h"
#include "RbMove.h"
#include "RbMoveSchedule.h"
#include "RbNames.h"


/**
 * @brief ConstantNode constructor from value
 *
 * This constructor creates a constant node from a value.
 *
 */
ConstantNode::ConstantNode(RbObject* val)
    : DAGNode(val) {
}


/**
 * @brief ConstantNode copy constructor
 *
 * This constructor creates a ConstantNode as a copy of another
 * ConstantNode.
 *
 * @param d     The constant node to clone.
 *
 */
ConstantNode::ConstantNode(const ConstantNode &d)
    :   DAGNode(d) {
}


/**
 * @brief Clone a ConstantNode
 *
 * This is the clone function for ConstantNode objects.
 *
 */
RbObject* ConstantNode::clone(void) const {

    return (ConstantNode*)(new ConstantNode(*this));
}


/** Get class vector describing type of object */
const StringVector& ConstantNode::getClass() const {

    static StringVector rbClass = StringVector(RbNames::ConstantNode::name) + DAGNode::getClass();
    return rbClass;
}


/**
 * @brief Thouch affected nodes
 *
 * This function touches all affected DAG nodes, i.e. marks them as changed.
 *
 */
void ConstantNode::touchAffectedChildren() {

}

/**
 * @brief Thouch affected nodes
 *
 * This function touches all affected DAG nodes, i.e. marks them as changed.
 *
 */
void ConstantNode::touchAffectedParents() {

}

/**
 * @brief Tell affected DAG nodes to keep current value
 *
 * This function calls all affected DAG nodes so that they
 * have a chance to keep the current value and discard the
 * previous value.
 *
 */
void ConstantNode::keepAffectedChildren() {

}

/**
 * @brief Tell affected DAG nodes to keep current value
 *
 * This function calls all affected DAG nodes so that they
 * have a chance to keep the current value and discard the
 * previous value.
 *
 */
void ConstantNode::keepAffectedParents() {

}

/**
 * @brief Restore affected nodes
 *
 * This function calls all nodes that are affected by this DAG node and restores
 * them.
 *
 */
void ConstantNode::restoreAffectedChildren() {

}

/**
 * @brief Restore affected nodes
 *
 * This function calls all nodes that are affected by this DAG node and restores
 * them.
 *
 */
void ConstantNode::restoreAffectedParents() {

}

/**
 * @brief Compare constant nodes
 *
 * To be the same, the constant nodes must have the same value and the same
 * children. It is the pointers to these objects we need to compare, not
 * their current values.
 *
 */
bool ConstantNode::equals(const RbObject* obj) const {

	const ConstantNode* c = dynamic_cast<const ConstantNode*>(obj);

    if (c == NULL)
        return false;

    if (value != c->value || storedValue != c->storedValue)
        return false;

    if (changed != c->changed || touchedProbability != c->touchedProbability || touchedLikelihood != c->touchedLikelihood)
        return false;

    if (children.size() != c->children.size() || parents.size() != c->parents.size())
        return false;

    for (std::set<DAGNode*>::iterator i=c->children.begin(); i!=c->children.end(); i++)
        if (children.find(*i) == children.end())
            return false;

    return true;
}

double ConstantNode::getLnProbabilityRatio() {
    return 0.0;
}

double ConstantNode::getLnProbability() {
    return 0.0;
}

RbObject& ConstantNode::operator=(const RbObject& obj) {

    try {
        // Use built-in fast down-casting first
        const ConstantNode& x = dynamic_cast<const ConstantNode&> (obj);

        ConstantNode& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const ConstantNode& x = dynamic_cast<const ConstantNode&> (*(obj.convertTo("const_node")));

            ConstantNode& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to const_node");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

DAGNode& ConstantNode::operator=(const DAGNode& obj) {

    try {
        // Use built-in fast down-casting first
        const ConstantNode& x = dynamic_cast<const ConstantNode&> (obj);

        ConstantNode& y = (*this);
        y = x;
        return y;
    } catch (std::bad_cast & bce) {
        try {
            // Try converting the value to an argumentRule
            const ConstantNode& x = dynamic_cast<const ConstantNode&> (*(obj.convertTo("const_node")));

            ConstantNode& y = (*this);
            y = x;
            return y;
        } catch (std::bad_cast & bce) {
            RbException e("Not supported assignment of " + obj.getClass()[0] + " to const_node");
            throw e;
        }
    }

    // dummy return
    return (*this);
}

ConstantNode& ConstantNode::operator=(const ConstantNode& obj) {

    changed = obj.changed;
    children = obj.children;
    (*lastMove) = (*obj.lastMove);
    monitors = obj.monitors;
    (*moves) = (*obj.moves);
    parents = obj.parents;
    (*storedValue) = (*obj.storedValue);
    touchedProbability = obj.touchedProbability;
    touchedLikelihood = obj.touchedLikelihood;
    (*value) = (*obj.value);

    return (*this);
}

/**
 * @brief Print constant node
 *
 * This function prints the content of the node.
 *
 * @param o     The ostream for printing.
 *
 */
void ConstantNode::printValue(std::ostream &o) const {

    o << value->toString() << std::endl;
}

std::string ConstantNode::toString(void) const {

    return value->toString();
}
