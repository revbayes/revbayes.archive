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
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"

#include <cassert>
#include <iostream>
#include <string>
#include <set>


/** Constructor from value */
ConstantNode::ConstantNode(RbObject* val) : DAGNode(val->getType()), value(val) {
}

/** Constructor from value class */
ConstantNode::ConstantNode(const std::string& valType) : DAGNode(valType), value(NULL) {
}

/** Copy constructor */
ConstantNode::ConstantNode(const ConstantNode& x) : DAGNode(x) {

    value = x.value->clone();
}

/** Destructor */
ConstantNode::~ConstantNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete node with references"); 
    delete value;
}

/** Assignment operator */
ConstantNode& ConstantNode::operator=(const ConstantNode& x) {

    if (this != &x) {

        if (valueType != x.valueType)
            throw RbException("Type mismatch");

        delete value;
        value = x.value->clone();
    }

    return (*this);
}

/** Clone this object */
ConstantNode* ConstantNode::clone(void) const {

    return new ConstantNode(*this);
}

/** Cloning the entire graph only involves children for a constant node */
ConstantNode* ConstantNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (ConstantNode*)(newNodes[(DAGNode*)(this)]);

    /* Make pristine copy */
    ConstantNode* copy = clone();
    newNodes[(DAGNode*)(this)] = copy;

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }
 
    return copy;
}

/** Get class vector describing type of DAG node */
const VectorString& ConstantNode::getDAGClass() const {

    static VectorString rbClass = VectorString(ConstantNode_name) + DAGNode::getDAGClass();
    return rbClass;
}

/** Get value element */
const RbObject* ConstantNode::getValElement(const VectorInteger& index) const {

    RbComplex* complexObject = dynamic_cast<RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    return complexObject->getElement(index);
}


/** Is it possible to mutate node to newNode? */
bool ConstantNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to mutate node to contain newValue? */
bool ConstantNode::isMutableTo(const VectorInteger& index, const RbObject* newValue) const {

    assert (!newValue->isType(Container_name));
    
    bool isMutable = false;

    return isMutable;
}


/** Mutate to newNode */
void ConstantNode::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
}


/* Mutate to contain newValue */
ConstantNode* ConstantNode::mutateTo(const VectorInteger& index, RbObject* newValue) {

    throw RbException("Not implemented yet");
}


/** Print value for user */
void ConstantNode::printValue(std::ostream& o) const {

    value->printValue(o);
}


/** Print struct for user */
void ConstantNode::printStruct(std::ostream &o) const {

    o << "Wrapper:" << std::endl;
    o << "&.class   = " << getDAGClass() << std::endl;
    o << "&.value   = " << value << std::endl;
    o << "&.parents = NULL" << std::endl;
    o << "&.children" << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Set Element */
void ConstantNode::setElement(const VectorInteger& index, RbObject* val) {

    RbComplex* complexObject = dynamic_cast<RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    complexObject->setElement(index, val);

    touchAffected();
}


/** Set value */
void ConstantNode::setValue(RbObject* val) {

    if ( val != NULL && !val->isType( getValueType() ) )
        throw RbException("Invalid assignment: type mismatch");

    if ( value != NULL )
        delete value;
    value = val;

    touchAffected();
}


/** Complete info on object */
std::string ConstantNode::toString(void) const {

    std::ostringstream o;
    o << "ConstantNode: value = ";
    value->printValue(o);

    return o.str();
}


/** Touch affected: only needed if a set function is called */
void ConstantNode::touchAffected(void) {

    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
}

