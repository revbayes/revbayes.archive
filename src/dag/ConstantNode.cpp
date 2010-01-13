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
#include "StringVector.h"

#include <iostream>
#include <string>
#include <set>


/** Constructor from value */
ConstantNode::ConstantNode(RbObject* val)
    : DAGNode(val->getType()), value(val) {
}


/** Constructor from value class */
ConstantNode::ConstantNode(const std::string& valType)
    : DAGNode(valType), value(NULL) {

}


/** Copy constructor */
ConstantNode::ConstantNode(const ConstantNode& x)
    : DAGNode(x) {

    value = x.value->clone();
}


/** Destructor */
ConstantNode::~ConstantNode() {

    if (children.size() != 0)
        throw RbException("Invalid delete: node with children");

    delete value;
}


/** Clone this object */
ConstantNode* ConstantNode::clone(void) const {

    return new ConstantNode(*this);
}


/** Get class vector describing type of object */
const StringVector& ConstantNode::getClass() const {

    static StringVector rbClass = StringVector(ConstantNode_name) + DAGNode::getClass();
    return rbClass;
}


/** Get value element */
const RbObject* ConstantNode::getValElement(const IntVector& index) const {

    RbComplex* complexObject = dynamic_cast<RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    return complexObject->getElement(index);
}


/** Print value for user */
void ConstantNode::printValue(std::ostream& o) const {

    value->printValue(o);
}


/** Print struct for user */
void ConstantNode::printStruct(std::ostream &o) const {

    o << "Wrapper:" << std::endl;
    o << "&.class = " << getClass() << std::endl;
    o << "&.value = " << getValue() << std::endl;
}


/** Set Element */
void ConstantNode::setElement(const IntVector& index, RbObject* val) {

    RbComplex* complexObject = dynamic_cast<RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    complexObject->setElement(index, val);

    touchAffected();
}


/** Set value */
void ConstantNode::setValue(RbObject* val) {

    if (val != NULL && !val->isType(getValueType()))
        throw RbException("Invalid assignment: type mismatch");

    if (value != NULL)
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
void ConstantNode::touchAffected(void) const {

    std::set<StochasticNode*> temp;
    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++)
        (*i)->getAffected(temp);
}
