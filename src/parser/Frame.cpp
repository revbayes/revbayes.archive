/**
 * @file
 * This file contains the implementation of Frame, which
 * is used to hold information about an evaluation or
 * execution frame.
 *
 * @brief Implementation of Frame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The REvBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNodeContainer.h"
#include "Frame.h"
#include "Parser.h"         // Capture parser debug flag
#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <sstream>


/** Construct frame with NULL parent */
Frame::Frame() :
    parentFrame(NULL), variableTable() {
}


/** Construct frame with parent */
Frame::Frame(Frame* parentFr) :
    parentFrame(parentFr), variableTable() {
}


/** Copy constructor */
Frame::Frame(const Frame& x) :
    parentFrame(x.parentFrame), variableTable(x.variableTable) {

    for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
        (*i).second.variable = (*i).second.variable->clone();
}


/** Destructor */
Frame::~Frame(void) {

    for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
        delete (*i).second.variable;
}


/** Assignment operator */
Frame& Frame::operator=(const Frame& x) {

    if (this != &x) {

        for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
            delete (*i).second.variable;

        variableTable = x.variableTable;
        for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
            (*i).second.variable = (*i).second.variable->clone();
    }

    return (*this);
}


/** Add "constant" variable object to table with initial value */
void Frame::addVariable(const std::string& name, RbObject* value) {

    /* Throw an error if the value is NULL */
    if (value == NULL)
        throw RbException("Cannot insert null value without type in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot;
    slot.type     = value->getType();
    slot.dim      = 0;
    slot.variable = new ConstantNode(value);

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted  variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), slot.variable->getValueType().c_str(), slot.variable->getDim());
}


/** Add simple variable object to table */
void Frame::addVariable(const std::string& name, DAGNode* var) {

    /* Throw an error if the variable is NULL */
    if (var == NULL)
        throw RbException("Cannot insert null variable without type in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot;
    slot.type     = var->getValueType();
    slot.dim      = var->getDim();
    slot.variable = var;

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), var->getValueType().c_str(), var->getDim());
}


/** Add container variable object to table */
void Frame::addVariable(const std::string& name, const IntVector& index, DAGNode* var) {

    /* Throw an error if the variable is NULL */
    if (var == NULL)
        throw RbException("Cannot insert null variable container without type in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    DAGNodeContainer* container = new DAGNodeContainer(index, var->getValueType());
    container->setElement(index, var);

    ObjectSlot slot;
    slot.type     = container->getValueType();
    slot.dim      = container->getDim();
    slot.variable = container;

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), container->getValueType().c_str(), container->getDim());
}


/** Add declared but empty slot to table */
void Frame::addVariable(const std::string& name, const std::string& type, int dim) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot;
    slot.type     = type;
    slot.dim      = dim;
    slot.variable = NULL;

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted null variable named '%s' of type '%s' and dim %d in frame\n",
        name.c_str(), type.c_str(), dim);
}


/** Clone entire environment, except base frame (it always stays the same) */
Frame* Frame::cloneEnvironment(void) const {

    Frame* newEnv = clone();
    if (newEnv->parentFrame != NULL && newEnv->parentFrame->getParentFrame() != NULL)
        newEnv->parentFrame = newEnv->parentFrame->cloneEnvironment();

    return newEnv;
}


/** Erase variable */
void Frame::eraseVariable(const std::string& name) {

    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end())
        throw (RbException("Variable " + name + " does not exist"));

    delete (*it).second.variable;
    variableTable.erase(it);

    PRINTF("Erased variable named '%s' in frame\n", name.c_str());
}


/** Does variable exist in the environment (current frame and enclosing frames)? */
bool Frame::existsVariable(const std::string& name) const {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->existsVariable(name);
        else
            return false;
    }

    return true;
}


/** Get value (read-only) */
const RbObject* Frame::getValue(const std::string& name) const {

    PRINTF("Retrieving value of variable named '%s' from frame\n", name.c_str());
    return getVariable(name)->getValue();
}


/** Get variable (read-only) */
const DAGNode* Frame::getVariable(const std::string& name) const {

    PRINTF("Retrieving variable named '%s' from frame\n", name.c_str());
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    return (*it).second.variable;
}


/** Get value element (read-only) */
const RbObject* Frame::getValElement(const std::string& name, const IntVector& index) const {

    // Find the variable
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getValElement(name, index);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        getValue(name);

    return (*it).second.variable->getValElement(index);
}


/** Get variable element (read-only) */
const DAGNode* Frame::getVarElement(const std::string& name, const IntVector& index) const {

    // Find the variable
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVarElement(name, index);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        getVariable(name);

    if (int(index.size()) != (*it).second.dim)
        throw (RbException("Subscript error"));

    return ((DAGNodeContainer*)((*it).second.variable))->getVarElement(index);
}


/** Print value for user */
void Frame::printValue(std::ostream& o) const {

    std::map<std::string, ObjectSlot>::const_iterator i;
    for (i=variableTable.begin(); i!=variableTable.end(); i++) {
        o << (*i).first << " = ";
        if ((*i).second.variable == NULL)
            o << "NULL";
        else
            (*i).second.variable->printValue(o);
        o << std::endl;
    }
}


/** Set value */
void Frame::setValue(const std::string& name, RbObject* value) {

    // Find the variable
    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setValue(name, value);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for setting it
    DAGNode* variable = (*it).second.variable;

    /* Check type */
    if (value != NULL && !value->isType((*it).second.type)) {
        std::ostringstream msg;
        msg << (*it).second.type;
        for (int i=0; i<(*it).second.dim; i++)
            msg << "[]";
        msg << " variable does not take ";
        msg << value->getType();
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Check for repeated assignment first*/
    if (variable != NULL && variable->getDim() > 0) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            if ((*container)[i]->isType(StochasticNode_name) && value != NULL)
                ((StochasticNode*)((*container)[i]))->clamp(value);
            else
                container->setElement(i, value);
        }
        return;
    }

    /* Simple assignment */
    if (variable != NULL && variable->isType(StochasticNode_name))
        ((StochasticNode*)(variable))->clamp(value);
    else {
        if (variable != NULL)
            delete variable;
        if (value == NULL)
            (*it).second.variable = NULL;
        else
            (*it).second.variable = new ConstantNode(value);
    }
}


/** Set variable */
void Frame::setVariable(const std::string& name, DAGNode* var) {

    // Find the variable
    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL) {
            parentFrame->setVariable(name, var);
            return;
        }
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for setting it
    DAGNode* variable = (*it).second.variable;

    // Special case if var is NULL
    if (var == NULL) {
        if ((*it).second.variable != NULL)
            delete (*it).second.variable;
        (*it).second.variable = NULL;
        return;
    }

    /* Check for repeated assignment first */
    if (variable != NULL && variable->getDim() > 0 && var->getDim() == 0 &&
        Workspace::userWorkspace().isXOfTypeY(var->getValueType(), variable->getValueType())) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            container->setElement(i, var);
        }
        return;
    }

    /* Check type and dim */
    if ((*it).second.dim != var->getDim() || !Workspace::userWorkspace().isXOfTypeY(var->getValueType(), (*it).second.type)) {
        std::ostringstream msg;
        msg << (*it).second.type;
        for (int i=0; i<(*it).second.dim; i++)
            msg << "[]";
        msg << " variable does not take ";
        msg << var->getValueType();
        for (int i=0; i<var->getDim(); i++)
            msg << "[]";
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Simple assignment */
    if (variable != NULL)
        delete variable;
    (*it).second.variable = var;
}


/** Set element of variable with object */
void Frame::setValElement(const std::string& name, const IntVector& index, RbObject* value) {

    /* Check for inadvertent calls */
    if (index.size() == 0)
        setValue(name, value);
 
    /* Find the variable */
    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->setValElement(name, index, value);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    /* We are responsible for setting it */
    DAGNode* variable = (*it).second.variable;

    /* Special case when variable is NULL */
    if ((value == NULL || value->isType((*it).second.type)) && index.size() == (*it).second.dim && variable == NULL) {
        if (value == NULL)
            variable = (*it).second.variable = new DAGNodeContainer(index, (*it).second.type);
        else
            variable = (*it).second.variable = new DAGNodeContainer(ContainerIterator(index), new ConstantNode(value));
        return;
    }

    /* Check dimension */
    if (int(index.size()) < variable->getDim() ||
        (int(index.size()) > variable->getDim() && !Workspace::userWorkspace().isXOfTypeY(variable->getValueType(), RbComplex_name)))
        throw (RbException("Subscript error"));

    /* Element assignment: variable checks types */
    variable->setElement(index, value);
}


/** Set element of variable with DAG node */
void Frame::setVarElement(const std::string& name, const IntVector& index, DAGNode* var) {

     /* Check for inadvertent calls */
    if (index.size() == 0)
        setVariable(name, var);
 
    /* Find the variable */
    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL) {
            parentFrame->setVarElement(name, index, var);
            return;
        }
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    /* We are responsible for setting it */
    DAGNode* variable = (*it).second.variable;

    /* Special case when variable is NULL */
    if ((var == NULL || Workspace::userWorkspace().isXOfTypeY(var->getValueType(), (*it).second.type)) &&
        index.size() == (*it).second.dim && variable == NULL) {
        if (var == NULL)
            variable = (*it).second.variable = new DAGNodeContainer(index, (*it).second.type);
        else
            variable = (*it).second.variable = new DAGNodeContainer(index, var);
        return;
    }

    /* Check index */
    if (int(index.size()) != (*it).second.dim)
        throw (RbException("Subscript error"));

    /* Disallow container assignment */
    if (var->getDim() > 0)
        throw RbException("Invalid assignment of container to variable element");

    /* Assignment: variable checks types */
    variable->setElement(index, var);
}


/** Complete info abot object to string */
std::string Frame::toString(void) const {

    std::ostringstream o;
    o << "Frame:" << std::endl;
    printValue(o);

    return o.str();
}

