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

const StringVector ConstantNode::rbClass = StringVector("const_node") + DAGNode::rbClass;

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
ConstantNode* ConstantNode::clone(void) const {

	return (new ConstantNode(*this));
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

    if (changed != c->changed || touched != c->touched)
        return false;

    if (children.size() != c->children.size() || parents.size() != c->parents.size())
        return false;

    for (std::set<DAGNode*>::iterator i=c->children.begin(); i!=c->children.end(); i++)
        if (children.find(*i) == children.end())
            return false;

    return true;
}


/**
 * @brief Print constant node
 *
 * This function prints the content of the node.
 *
 * @param o     The ostream for printing.
 *
 */
void ConstantNode::print(std::ostream& o) const {

	o << "ConstantNode: ";
    //   o << getClass()->print(o);
    o << std::endl;

    DAGNode::print(o);
}

void ConstantNode::printValue(std::ostream &o) const {

    o << value->toString() << std::endl;
}

std::string ConstantNode::toString(void) const {

    return value->toString();
}
