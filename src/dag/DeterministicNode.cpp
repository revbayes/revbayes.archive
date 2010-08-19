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

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "VectorString.h"

#include <algorithm>


/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode(const std::string& type) : VariableNode(type), changed(false), function(NULL), value(NULL), storedValue(NULL)  {
	
	isDagExposed = false;
}


/** Constructor from function: get parents from the function object */
DeterministicNode::DeterministicNode(RbFunction* func) : VariableNode(func->getReturnType())  {

    /* Check for cycles */
    const std::vector<DAGNode*>& arguments = func->getProcessedArguments();
    std::list<DAGNode*> done;
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        if ((*i)->isParentInDAG(this, done))
            throw RbException ("Invalid assignment: cycles in the DAG");
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
    storedValue = function->execute()->clone();
    value = storedValue;
	
	/* set whether to expose this DAG to the user */
	isDagExposed = false;
}


/** Copy constructor */
DeterministicNode::DeterministicNode(const DeterministicNode& x) : VariableNode(x) {

    changed      = x.changed;
    function     = (RbFunction*)(x.function->clone());
    storedValue  = function->execute()->clone();
    value        = storedValue;
	isDagExposed = x.isDagExposed;

    /* Set parents and add this node as a child node of these */
    const std::vector<DAGNode*>& arguments = function->getProcessedArguments();
    for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
        parents.insert(*i);
        (*i)->addChildNode(this);
    }
}


/** Destructor: do not delete value because it only points to another object */
DeterministicNode::~DeterministicNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete node with references");

    /* Remove this node as a child node of parents and delete these if appropriate */
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
        (*i)->removeChildNode(this);
        if ((*i)->numRefs() == 0)
            delete (*i);
    }

    delete function;
    delete storedValue;
}


/** Assignment operator */
DeterministicNode& DeterministicNode::operator=(const DeterministicNode& x) {

    if (this != &x) {

        if (valueType != x.valueType)
            throw RbException("Type mismatch");
        
        changed     = x.changed;

        /* Remove this node as a child node of parents */
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++) {
            (*i)->removeChildNode(this);
            if ((*i)->numRefs() == 0)
                delete (*i);
        }

        delete function;
        function    = (RbFunction*)(x.function->clone());

        delete storedValue;
        storedValue = function->execute()->clone();
        value       = storedValue;

        /* Set parents and add this node as a child node of these */
        parents.clear();
        const std::vector<DAGNode*>& arguments = function->getProcessedArguments();
        for (std::vector<DAGNode*>::const_iterator i=arguments.begin(); i!=arguments.end(); i++) {
            parents.insert(*i);
            (*i)->addChildNode(this);
        }
    }

    return (*this);
}


/** Clone this object */
DeterministicNode* DeterministicNode::clone(void) const {

	return new DeterministicNode(*this);
}


/** Clone the entire graph: clone children, swap parents */
DeterministicNode* DeterministicNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (DeterministicNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    DeterministicNode* copy = new DeterministicNode(valueType);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone parents */
    copy->function = (RbFunction*)(function->clone());
    std::vector<DAGNode*>& params     = const_cast<std::vector<DAGNode*>& > (function->getProcessedArguments());
    std::vector<DAGNode*>& copyParams = const_cast<std::vector<DAGNode*>& > (copy->function->getProcessedArguments());
    copyParams.clear();
    copy->storedValue = function->execute()->clone();   // Make sure we have a copy of the value
    copy->value = copy->storedValue;
    copy->touched = false;
    copy->changed = false;
    for (std::vector<DAGNode*>::iterator i=params.begin(); i!=params.end(); i++) {
        DAGNode* theParentClone = (*i)->cloneDAG(newNodes);
        copyParams.push_back(theParentClone);
        copy->parents.insert(theParentClone);
        theParentClone->addChildNode(copy);
    }

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
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


/** Get class vector describing type of object */
const VectorString& DeterministicNode::getClass() const {

    static VectorString rbClass = VectorString(DeterministicNode_name) + VariableNode::getClass();
    return rbClass;
}


/* Get default moves, if any */
MoveSchedule* DeterministicNode::getDefaultMoves(void) {

    const RbComplex* complexValue = dynamic_cast<const RbComplex*>(storedValue);
    if (complexValue != NULL)
        return complexValue->getDefaultMoves(this);
    else
        return NULL;
}


/** Get stored value intelligently */
const RbObject* DeterministicNode::getStoredValue(void) {

    if (!touched)
        return value;

    update();
    return storedValue;
}


/** Get value intelligently: value is a passive pointer that points to value in function or storedValue */
const RbObject* DeterministicNode::getValue(void) {

    update();
    return value;
}


/** Get const value if possible */
const RbObject* DeterministicNode::getValue(void) const {

    if (touched && !changed)
        throw RbException("Const value not available");

    return value;
}


/** Get value element */
const RbObject* DeterministicNode::getValElement(const VectorInteger& index) const {

    const RbComplex* complexObject = dynamic_cast<const RbComplex*>(value);
    if (complexObject == NULL)
        throw RbException("Object does not have elements");

    return complexObject->getElement(index);
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


/** Print struct for user */
void DeterministicNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "Wrapper:" << std::endl;
    o << "&.class    = " << getClass() << std::endl;
    o << "&.function = " << function << std::endl;
    o << "&.value    = " << value << std::endl;
    o << "&.parents = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "&.children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Print value for user */
void DeterministicNode::printValue(std::ostream& o) const {

    if (touched && !changed)
        throw RbException("Cannot print value while in touched state");

    value->printValue(o);
}


/** Restore value of node and affected variable nodes */
void DeterministicNode::restoreAffected(void) {

    if (touched) {
        if (changed)
         	value = storedValue;        // Remember, value is just a pointer
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++) {
            (*i)->restoreAffected();
        }
    }
    touched = changed = false;
}


/** Set element: throw error */
void DeterministicNode::setElement(const VectorInteger& index, RbObject* val) {

    throw RbException("Cannot set element of deterministic DAG node");
}


/** Set value: throw error */
void DeterministicNode::setValue(RbObject* val) {

    throw RbException("Cannot set value of deterministic DAG node");
}


/** Swap parent node both in parents set and in processed arguments */
void DeterministicNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not a parent");
    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    std::vector<DAGNode*>& params = const_cast<std::vector<DAGNode*>& > (function->getProcessedArguments());
    std::vector<DAGNode*>::iterator it = std::find(params.begin(), params.end(), oldNode);
    if (it == params.end())
        throw RbException("Node is not a parameter");
    it = params.erase(it);
    params.insert(it, newNode);

    touched = true;
    changed = false;
    touchAffected();
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


/** Tell affected nodes that upstream value has been reset */
void DeterministicNode::touchAffected(void) {

    touched = true;
    changed = false;
    for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
        (*i)->touchAffected();
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

