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
#include "ObjectSlot.h"
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

    ConstantNode* var = new ConstantNode(value);

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, ObjectSlot(var)));

    PRINTF("Inserted  variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), var->getValueType().c_str(), var->getDim());
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

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, ObjectSlot(var)));

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

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, container));

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

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, ObjectSlot(type, dim)));

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

    if (variableTable.find(name) == variableTable.end())
        throw (RbException("Variable " + name + " does not exist"));

    PRINTF("Erasing variable named '%s' in frame\n", name.c_str());
    variableTable.erase(name);
}


/** Does variable exist in the environment (current frame and enclosing frames)? */
bool Frame::existsVariable(const std::string& name) const {

    PRINTF("Testing if variable exists");
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
    std::map<const std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    return (*it).second.getVariable();
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

    return (*it).second.getVariable()->getValElement(index);
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

    if (int(index.size()) != (*it).second.getDim())
        throw (RbException("Subscript error"));

    return ((DAGNodeContainer*)((*it).second.getVariable()))->getVarElement(index);
}


/** Print value for user */
void Frame::printValue(std::ostream& o) const {

    std::map<std::string, ObjectSlot>::const_iterator i;
    for (i=variableTable.begin(); i!=variableTable.end(); i++) {
        o << (*i).first << " = ";
        (*i).second.printValue(o);
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
    DAGNode* variable = (*it).second.getVariable();

    /* Check for repeated assignment first */
    if (variable->getDim() > 0 && (value == NULL || value->isType(variable->getValueType()))) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            if ((*container)[i]->isType(StochasticNode_name) && value != NULL)
                ((StochasticNode*)((*container)[i]))->clamp(value);
            else {
                if ((*container)[i] != NULL)
                    delete (*container)[i];
                if (value == NULL)
                    (*container)[i] = new ConstantNode(container->getValueType());
                else
                    (*container)[i] = new ConstantNode(value);
            }
        }
        return;
    }

    /* Check type and dim */
    if (value != NULL && (!value->isType(variable->getValueType()) || !variable->getDim() == 1)) {
        std::ostringstream msg;
        msg << (*it).second.getTypeDescr() << " variable does not take ";
        msg << value->getType();
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Simple assignment */
    if (variable->isType(StochasticNode_name) && value != NULL)
        ((StochasticNode*)(variable))->clamp(value);
    else {
        if (value == NULL)
            (*it).second.setVariable(NULL);
        else
            (*it).second.setVariable(new ConstantNode(value));
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
    DAGNode* variable = (*it).second.getVariable();

    // Special case if var is NULL
    if (var == NULL) {
        (*it).second.setVariable(NULL);
        return;
    }

    /* Check for repeated assignment first */
    if (variable->getDim() > 0 && var->getDim() == 0 && Workspace::userWorkspace().isXOfTypeY(var->getValueType(), variable->getValueType())) {

        /* We want to do repeated assignment */
        DAGNodeContainer* container = (DAGNodeContainer*)(variable);
        for (ContainerIterator i=container->begin(); i!=container->end(); i++) {
            if ((*container)[i] == NULL)
                delete (*container)[i];
            (*container)[i] = (DAGNode*)(var);
        }
        return;
    }

    /* Check type and dim */
    if (variable->getDim() != var->getDim() || !Workspace::userWorkspace().isXOfTypeY(var->getValueType(), variable->getValueType())) {
        std::ostringstream msg;
        msg << (*it).second.getTypeDescr() << " variable does not take ";
        msg << var->getType();
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
    DAGNode* variable = (*it).second.getVariable();

    /* Check dimension */
    if (int(index.size()) < variable->getDim() ||
        (int(index.size()) > variable->getDim() && !Workspace::userWorkspace().isXOfTypeY(variable->getValueType(), RbComplex_name)))
        throw (RbException("Subscript error"));

    /* Divide index in variable index and value index */
    IntVector variableIndex, valueIndex;
    for (size_t i=0; i<index.size(); i++) {
        if (int(i) < variable->getDim())
            variableIndex.push_back(index[i]);
        else
            valueIndex.push_back(index[i]);
    }

    /* Assignment: variable checks types */
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
    DAGNode* variable = (*it).second.getVariable();

    /* Check index */
    if (int(index.size()) != variable->getDim())
        throw (RbException("Subscript error"));

    /* Disallow container assignment */
    if (!var->isType(DAGNode_name))
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



