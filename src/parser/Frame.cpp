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

#include "DAGNodeContainer.h"
#include "Frame.h"
#include "RbException.h"


/** Constructor from parent frame; default is NULL */
Frame::Frame(Frame* parentFr) :
    parentFrame(parentFr), variableTable() {
}


/** Add simple variable object to table */
void Frame::addVariable(const std::string& name, DAGNode* variable) {

    // Throw an error if the variable exists
    // Note that we cannot use existsVariable because that function looks in
    // parent frames recursively, so then we cannot "overload" variable names locally
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot = ObjectSlot(variable);
    variableTable.insert(std::pair<const std::string, ObjectSlot>(name, slot));
}


/** Add container variable object to table */
void Frame::addVariable(const std::string& name, const IntVector& index, DAGNode* variable) {

    // Throw an error if the variable exists
    // Note that we cannot use existsVariable because that function looks in
    // parent frames recursively, so then we cannot "overload" variable names locally
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    DAGNodeContainer* container = new DAGNodeContainer(index, variable->getValue()->getType());
    container->setElement(index, variable);

    ObjectSlot slot = ObjectSlot(container);
    variableTable.insert(std::pair<const std::string, ObjectSlot>(name, slot));
}


/** Add declared but empty slot to table */
void Frame::addVariable(const std::string& name, const std::string& type, int dim) {

    // Throw an error if the variable exists
    // Note that we cannot use existsVariable because that function looks in
    // parent frames recursively, so then we cannot "overload" variable names locally
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot = ObjectSlot(type, dim);
    variableTable.insert(std::pair<const std::string, ObjectSlot>(name, slot));
} 


/** Erase variable */
void Frame::eraseVariable(const std::string& name) {

    if (variableTable.find(name) == variableTable.end())
        throw (RbException("Variable " + name + " does not exist"));

    variableTable.erase(name);
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


/** Get declared type of variable */
const std::string& Frame::getDeclaredType(const std::string& name) const {

    std::map<const std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end())
        throw (RbException("Variable " + name + " does not exist"));

    return (*it).second.getType();
}


/** Get variable (a copy) */
RbObject* Frame::getVariable(const std::string& name) {

    std::map<const std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    return (*it).second.getValue()->clone();
}

