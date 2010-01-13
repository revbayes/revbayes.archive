/**
 * @file
 * This file contains the implementation of DeterministicNode, which is derived
 * from VariableNode. DeterministicNode is used for DAG nodes associated with an
 * expression (equation) that determines their value.
 *
 * @brief Implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ContainerIterator.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "StringVector.h"


/** Constructor from function: get parents from the function object */
DeterministicNode::DeterministicNode(RbFunction* func) : VariableNode(func->getReturnType())  {

    /* Check for cycles */
    const std::vector<DAGNode*>& arguments = func->getProcessedArguments();
    std::list<DAGNode*> done;
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        if ((*i)->isParentInDAG(this, done))
            throw RbException ("Invalid assignment: Cycles in the DAG");
    }

    /* Set parents and add this node as a child node of these */
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }

    /* Set flag */
    changed = false;

    /* Set the function */
    function = func;

    /* Set value and stored value */
    value = function->execute();
    storedValue = value->clone();

    /* Get default block moves, if any */
    RbComplex* complexValue = dynamic_cast<RbComplex*>(storedValue);
    if (complexValue != NULL) {
        moves = complexValue->getDefaultMoves(this);
    }
}


/** Copy constructor */
DeterministicNode::DeterministicNode(const DeterministicNode& x) : VariableNode(x) {

    changed     = x.changed;
    function    = (RbFunction*)(x.function->clone());
    value       = function->execute();
    storedValue = value->clone();
}


/** Destructor: do not delete value because it only points to another object */
DeterministicNode::~DeterministicNode(void) {

    delete function;
    delete storedValue;
}


/** Assignment operator */
DeterministicNode& DeterministicNode::operator=(const DeterministicNode& x) {

    if (this != &x) {
        VariableNode::operator=(x);

        changed     = x.changed;

        delete function;
        function    = (RbFunction*)(x.function->clone());

        delete storedValue;
        value       = function->execute();
        storedValue = value->clone();

        //blockMoves = x.blockMoves;
    }

    return (*this);
}


/** Clone this object */
DeterministicNode* DeterministicNode::clone(void) const {

	return new DeterministicNode(*this);
}


/** Get class vector describing type of object */
const StringVector& DeterministicNode::getClass() const {

    static StringVector rbClass = StringVector(DeterministicNode_name) + VariableNode::getClass();
    return rbClass;
}


/** Get affected nodes: touch and pass through to next stochastic node */
void DeterministicNode::getAffected(std::set<StochasticNode*>& affected) {

    /* If we have already touched this node, we are done; otherwise, get the affected children */
    if (!touched) {
        touched = true;
        changed = false;
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->getAffected(affected);
        }
    }
}


/** Get stored value intelligently */
const RbObject* DeterministicNode::getStoredValue(void) {

    update();
    return storedValue;
}


/** Get value intelligently: value is a passive pointer that points to value in function or storedValue */
const RbObject* DeterministicNode::getValue(void) {

    update();
    return value;
}


/** Get value element */
const RbObject* DeterministicNode::getValElement(const IntVector& index) const {

    const RbComplex* complexObject = dynamic_cast<const RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    return complexObject->getElement(index);
}


/** Get value unconditionally: throw an error if touched and not changed */
const RbObject* DeterministicNode::getValue(void) const {

    if (touched && !changed)
        throw RbException("Value is out of date");

    return value;
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keepAffected(void) {

    if (touched) {
        if (!changed)
            update();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->keepAffected();
        }
    }
    touched = changed = false;
}


/** Print value for user */
void DeterministicNode::printValue(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print value while in touched state");

    value->printValue(o);
}


/** Print struct for user */
void DeterministicNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class    = " << getClass() << std::endl;
    o << "&.function = " << function << std::endl;
    o << "&.value    = " << getValue() << std::endl;
    o << std::endl;
}


/** Restore value of node and affected variable nodes */
void DeterministicNode::restoreAffected(void) {

    if (touched) {
        if (!changed)
            update();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->restoreAffected();
        }
     	value = storedValue;        // Remember, value is just a pointer
    }
    touched = changed = false;
}


/** Set element: throw error */
void DeterministicNode::setElement(const IntVector& index, RbObject* val) {

    throw RbException("Cannot set element of deterministic DAG node");
}


/** Set value: throw error */
void DeterministicNode::setValue(RbObject* val) {

    throw RbException("Cannot set value of deterministic DAG node");
}


/** Complete info about object */
std::string DeterministicNode::toString(void) const {

    std::ostringstream o;

    o << "DeterministicNode:" << std::endl;

    o << "name        = " << name << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;

    o << "function    = ";
    function->printValue(o);
    o << std::endl;

    o << "value       = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void DeterministicNode::update(void) {

    if (touched && !changed) {
        if (value != storedValue) {
            if (storedValue != NULL)
                delete storedValue;
            storedValue = value->clone();
        }
        value = function->execute();
        changed = true;
    }
}


