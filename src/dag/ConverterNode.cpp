/**
 * @file
 * This file contains the implementation of ConverterNode.
 *
 * @brief Implementation of ConverterNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ConverterNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <algorithm>
#include <cassert>


/** Constructor of pristine converter node */
ConverterNode::ConverterNode(const std::string& type, int dim)
    : DeterministicNode(type), valueDim(dim) {
}


/** Basic constructor of converter node */
ConverterNode::ConverterNode(DAGNode* origNode, const TypeSpec& typeSpec)
    : DeterministicNode(typeSpec.getType()), valueDim(typeSpec.getDim()) {

    /* Check for cycles */
    std::list<DAGNode*> done;
    if (origNode->isParentInDAG(this, done))
        throw RbException ("Invalid assignment: cycles in the DAG");

    /* Set parent and add this node as a child */
    parents.insert(origNode);
    origNode->addChildNode(this);

    /* Set value and stored value */
    value       = origNode->getValue()->convertTo(valueType, valueDim);
    storedValue = NULL;
}


/** Clone this object */
ConverterNode* ConverterNode::clone(void) const {

	return new ConverterNode(*this);
}


/** Clone the entire graph: clone children, swap parent */
ConverterNode* ConverterNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (ConverterNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    ConverterNode* copy = new ConverterNode(valueType, valueDim);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone parents */
    copy->value = copy->value->clone();
    copy->storedValue = NULL;
    copy->touched = false;
    copy->changed = false;
    DAGNode* theParentClone = (*parents.begin())->cloneDAG(newNodes);
    copy->parents.insert(theParentClone);
    theParentClone->addChildNode(copy);

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& ConverterNode::getDAGClass() const {

    static VectorString rbClass = VectorString(ConverterNode_name) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Is it possible to mutate node to newNode? */
bool ConverterNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to change parent node oldNode to newNode? */
bool ConverterNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find out if node is parent
    if (*parents.begin() != const_cast<DAGNode*>(oldNode))
        throw RbException("Node is not a parent");

    // See if the new node value is convertible to the required type
    if ( newNode->getValue()->isConvertibleTo(valueType, valueDim, false) )
        return true;
    
    return false;
}


/** Mutate to newNode */
void ConverterNode::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
}


/** Print struct for user */
void ConverterNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "DAGNode:" << std::endl;
    o << "_class    = " << getDAGClass() << std::endl;
    o << "_value    = " << value << std::endl;
    o << "_parent   = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "_children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Complete info about object */
std::string ConverterNode::richInfo(void) const {

    std::ostringstream o;

    o << "ConverterNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "valueDim    = " << valueDim << std::endl;

    o << "value       = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Swap parent node */
void ConverterNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    touched = true;
    changed = false;
    touchAffected();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void ConverterNode::update(void) {

    if (touched && !changed) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        value = (*parents.begin())->getValue()->convertTo(valueType, valueDim);
        changed = true;
    }
}

