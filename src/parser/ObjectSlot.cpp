/**
 * @file
 * This file contains the implementation of ObjectSlot, which is
 * a slot for a variable. It contains a specification of the
 * declared (required) atomicType and dimensions of the object, in
 * addition to the current value.
 *
 * @brief Implementation of ObjectSlot
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */


#include "ConstantNode.h"
#include "DAGNodeContainer.h"
#include "IntVector.h"
#include "ObjectSlot.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbObjectWrapper.h"
#include "StochasticNode.h"
#include "StringVector.h"

#include <sstream>


/** Constructor with NULL init */
ObjectSlot::ObjectSlot(const std::string valType, int elemDim)
    : atomicType(valType), dim(elemDim), variable(NULL) {
}


/** Constructor with value init */
ObjectSlot::ObjectSlot(RbObject* initValue) {

    atomicType = initValue->getType();
    dim        = initValue->getDim();
    variable   = new ConstantNode(initValue);
}


/** Constructor with variable init */
ObjectSlot::ObjectSlot(RbObjectWrapper* initVariable) {

    atomicType = initVariable->getValue()->getType();
    dim        = initVariable->getDim();
    variable   = initVariable;

    // If the wrapper does not have dimensions, we expose the value dimensions
    if (dim == 0)
        dim = initVariable->getValue()->getDim();
}


/** Copy constructor */
ObjectSlot::ObjectSlot(const ObjectSlot& x) {
    printf("Copy\n");
    atomicType = x.atomicType;
    dim        = x.dim;
    std::cerr << x.variable;
    x.variable->printValue(std::cerr);
    variable = NULL;
    printf ("Variable is NULL\n");
    x.variable->clone();
    printf ("Variable is cloned\n");

    variable   = x.variable->clone();
}


/** Assignment operator */
ObjectSlot& ObjectSlot::operator=(const ObjectSlot& x) {
    printf("Assignment\n");
    if (this != &x) {
        atomicType = x.atomicType;
        dim        = x.dim;
        variable   = x.variable->clone();
    }

    return (*this);
}


/** Get class vector describing type of object */
const StringVector& ObjectSlot::getClass() const {

    static StringVector rbClass = StringVector(ObjectSlot_name) + RbInternal::getClass();
    return rbClass;
}


/** Get description of atomic type with indication of dimensions with empty square brackets */
std::string ObjectSlot::getTypeDescr() const {

    std::string s = atomicType;
    for (int i=0; i<dim; i++)
        s += "[]";

    return s;
}


/** Get value element */
const RbObject* ObjectSlot::getValElement(const IntVector& index) const {

    if (variable == NULL)
        return NULL;

    if (index.size() == 0)
        getValue();

    if (index.size() != size_t(dim))
        throw (RbException("Subscript error"));

    return variable->getValElement(index);
}


/** Get value */
const RbObject* ObjectSlot::getValue(void) const {

    if (variable == NULL)
        return NULL;
    else
        return variable->getValue();
}


/** Get variable element */
const RbObjectWrapper* ObjectSlot::getVarElement(const IntVector& index) const {

    if (index.size() == 0)
        getVariable();

    if (int(index.size()) != dim)
        throw (RbException("Subscript error"));

    return variable->getVarElement(index);
}


/** Does a value or variable with specified class vector fit the atomic type of the slot? */
bool ObjectSlot::isMatchingType(const StringVector& classVec) const {

    for (size_t i=0; i<classVec.size(); i++) {
        if (classVec[i] == atomicType)
            return true;
    }
    return false;
}


/** Print value for user */
void ObjectSlot::printValue(std::ostream& o) const {

    if (variable == NULL)
        o << "NULL";
    else
        variable->printValue(o);
}


/** Set value of variable */
void ObjectSlot::setValue(RbObject* value) {

    /* Check for repeated assignment first */
    if (variable != NULL && value != NULL &&
        variable->getWrapperDim() > 0 &&
        variable->getWrapperDim() + value->getDim() == dim &&
        isMatchingType(value->getAtomicClass())) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            if ((*container)[i]->isType(StochasticNode_name))
                ((StochasticNode*)((*container)[i]))->clamp(value);
            else {
                if ((*container)[i] != NULL)
                    delete (*container)[i];
                (*container)[i] = new ConstantNode(value);
            }
        }
        return;
    }

    /* Check atomic type and dim */
    if (value != NULL && (!value->isType(atomicType) || value->getDim() != dim)) {
        std::stringstream msg;
        msg << getTypeDescr() << " slot does not take ";
        msg << value->getAtomicType();
        for (int i=0; i<value->getDim(); i++)
            msg << "[]";
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Simple assignment */
    if (variable != NULL && variable->isType(StochasticNode_name))
        ((StochasticNode*)(variable))->clamp(value);
    else {
        if (variable != NULL)
            delete variable;
        variable = new ConstantNode(value);
    }
}


/** Replace variable (if atomic type and dim are right) */
void ObjectSlot::setVariable(RbObjectWrapper* var) {
    
    /* Check for repeated assignment first */
    if (variable != NULL && var != NULL &&
        variable->getWrapperDim() > 0 &&
        var->getWrapperDim() == 0 &&
        variable->getWrapperDim() + var->getDim() == dim &&
        isMatchingType(var->getAtomicClass())) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            if ((*container)[i] != NULL)
                delete (*container)[i];
            (*container)[i] = (DAGNode*)(var);
        }
        return;
    }

    /* Check atomic type and dim */
    if (var != NULL && (!isMatchingType(var->getAtomicClass()) || var->getDim() != dim)) {
        std::stringstream msg;
        msg << getTypeDescr() << " slot does not take ";
        msg << var->getAtomicClass()[0];
        for (int i=0; i<var->getDim(); i++)
            msg << "[]";
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Simple assignment */
    if (variable != NULL)
        delete variable;
    variable = var;
}


/** Set value of variable element */
void ObjectSlot::setValElement(const IntVector& index, RbObject* val) {

    /* Check for inadvertent calls */
    if (index.size() == 0)
        setValue(val);
 
    /* Check dimension */
    if (int(index.size()) + val->getDim() != dim)
        throw (RbException("Subscript error"));

    /* Check atomic type */
    if (val!=NULL && !isMatchingType(val->getAtomicClass())) {
        std::stringstream msg;
        msg << getTypeDescr() << " slot does not take ";
        msg << val->getAtomicType();
        for (int i=0; i<val->getDim(); i++)
            msg << "[]";
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Create new DAG node container if slot is empty */
    if (variable == NULL) {
        variable = new DAGNodeContainer(index, new ConstantNode(val));
        return;
    }

    /* Assignment to existing DAG node container */
    if (int(index.size()) < variable->getWrapperDim())
        throw (RbException("Invalid assignment to DAG node subcontainer"));
    variable->setElement(index, val);
}


/** Set variable element */
void ObjectSlot::setVarElement(const IntVector& index, RbObjectWrapper* var) {

     /* Check for inadvertent calls */
    if (index.size() == 0)
        setVariable(var);
 
    /* Check index */
    if (int(index.size()) + var->getDim() != dim)
        throw (RbException("Subscript error"));

    /* Check atomic type */
    if (!isMatchingType(var->getAtomicClass())) {
        std::stringstream msg;
        msg << getTypeDescr() << " slot does not take ";
        msg << var->getAtomicClass()[0];
        for (int i=0; i<var->getDim(); i++)
            msg << "[]";
        msg << " value";
        throw (RbException(msg.str()));
    }

   /* Create new DAG node container if slot is empty */
    if (variable == NULL) {
        DAGNode* node = (DAGNode*)(var);
        if (node == NULL)
            throw (RbException("Invalid assignment of variable container to variable container element")); 
        variable = new DAGNodeContainer(index, node);
        return;
    }

    /* Assignment to existing DAG node container */
    if (variable->getWrapperDim() == 0)
        throw (RbException("Invalid assignment of variable to constant element"));
    if (int(index.size()) < variable->getWrapperDim())
        throw (RbException("Invalid assignment to DAG node subcontainer"));
    variable->setElement(index, var);
}


/** Complete info about object */
std::string ObjectSlot::toString(void) const {

    std::ostringstream o;
	o << "ObjectSlot: " << getTypeDescr() << " = ";
    if (variable == NULL)
        o << "NULL";
    else
        variable->printValue(o);

    return o.str();
}


