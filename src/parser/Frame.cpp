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

#include <sstream>


/** Construct frame with NULL parent */
Frame::Frame() :
    parentFrame(NULL), variableTable() {
}


/** Construct frame with parent */
Frame::Frame(Frame* parentFr) :
    parentFrame(parentFr), variableTable() {
}


/** Copy constructor (we copy frame, not environment) */
Frame::Frame(const Frame& x) {

    parentFrame = x.parentFrame;
    for (std::map<std::string, RbObjectWrapper*>::iterator i=x.variableTable.begin(); i!=x.variableTable.end(), i++)
        variableTable.insert((*i).second->clone());
}


/** Destructor (we delete frame, not environment) */
Frame::~Frame() {

    for (std::map<std::string, RbObjectWrapper*>::iterator i=x.variableTable.begin(); i!=x.variableTable.end(), i++)
        delete (*i).second;
}


/** Assignment operator */
Frame& Frame::operator=(const Frame& x) {

    if (this != &x) {
        parentFrame = x.parentFrame;    // Do not destroy parent

        for (std::map<std::string, RbObjectWrapper*>::iterator i=variableTable.begin(); i!=variableTable.end(), i++)
            delete (*i).second;
        for (std::map<std::string, RbObjectWrapper*>::iterator i=x.variableTable.begin(); i!=x.variableTable.end(), i++)
            variableTable.insert((*i).second->clone());
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

    ConstantNode* var = new ConstantNode(value);

    PRINTF("Inserting variable named '%s' of type '%s' in frame\n", name.c_str(), var->getTypeDescr().c_str());
    variableTable.insert(std::pair<std::string, RbObjectWrapper*>(name, new ConstantNode(value)));
}


/** Add simple variable object to table */
void Frame::addVariable(const std::string& name, RbObjectWrapper* var) {

    /* Throw an error if the variable is NULL */
    if (var == NULL)
        throw RbException("Cannot insert null variable without type in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    PRINTF("Inserting variable named '%s' of type '%s' in frame\n", name.c_str(), var->getTypeDescr().c_str());
    variableTable.insert(std::pair<std::string, RbObjectWrapper*>(name, variable));
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

    PRINTF("Inserting variable named '%s' of type '%s' in frame\n", name.c_str(), container->getTypeDescr().c_str());
    variableTable.insert(std::pair<std::string, RbObjectWrapper*>(name, container));
}


/** Add declared but empty slot to table */
void Frame::addVariable(const std::string& name, const std::string& type, int dim) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    PRINTF("Inserting null variable named '%s' of type '%s' in frame\n",
            name.c_str(), getTypeDescr(type, dim).c_str());

    RbObjectWrapper* variable;
    if (dim == 0)
        variable = new ConstantNode(type, dim);
    else
        variable = new DAGNodeContainer(type, dim);    
    variableTable.insert(std::pair<std::string, RbObjectWrapper*>(name, variable));
} 


/** Clone entire environment, except base frame (it always stays the same) */
Frame* Frame::cloneEnvironment(void) const {

    if (parentFrame != NULL && parentFrame->getParentFrame() != NULL)
        parentFrame = parentFrame->cloneEnvironment();

    return clone();
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


/** Get description of type with indication of dimensions using empty square brackets */
std::string Frame::getTypeDescr(std::string& type, int dim) const {

    std::string s = type;
    for (int i=0; i<dim; i++)
        s += "[]";

    return s;
}


/** Get value (read-only) */
const RbObject* Frame::getValue(const std::string& name) const {

    PRINTF("Retrieving value of variable named '%s' from frame\n", name.c_str());
    return getVariable(name)->getValue(); 
}


/** Get variable (read-only) */
const RbObjectWrapper* Frame::getVariable(const std::string& name) const {

    PRINTF("Retrieving variable named '%s' from frame\n", name.c_str());
    std::map<const std::string, RbObjectWrapper*>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    return (*it).second;
}


/** Get value element (read-only) */
const RbObject* Frame::getValElement(const std::string& name, const IntVector& index) const {

    // Find the variable
    std::map<std::string, RbObjectWrapper*>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getValElement(name, index);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        getValue(name);

    return (*it).second->getValElement(index);
}


/** Get variable element (read-only) */
const RbObjectWrapper* Frame::getVarElement(const std::string& name, const IntVector& index) const {

    // Find the variable
    std::map<std::string, RbObjectWrapper*>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVarElement(name, index);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        getVariable(name);

    if (int(index.size()) != (*it).second->getDim())
        throw (RbException("Subscript error"));

    return ((DAGNodeContainer*)((*it).second))->getVarElement(index);
}


/** Print value for user */
void Frame::printValue(std::ostream& o) const {

    std::map<std::string, RbObjectWrapper*>::const_iterator i;
    for (i=variableTable.begin(); i!=variableTable.end(); i++) {
        o << i->first << " = ";
        i->second.printValue(o);
        o << std::endl;
    }
}


/** Set value */
void Frame::setValue(const std::string& name, RbObject* value) {

    // Find the variable
    std::map<std::string, RbObjectWrapper*>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setValue(name, value);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for setting it
    RbObjectWrapper* variable = (*it).second;

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
                    (*container)[i] = new ConstantNode(container->getValueClass());
                else
                    (*container)[i] = new ConstantNode(value);
            }
        }
        return;
    }

    /* Check type and dim */
    if (value != NULL && (!value->isType(variable->getValueType()) || !variable->getDim() == 1)) {
        std::ostringstream msg;
        msg << variable->getTypeDescr() << " variable does not take ";
        msg << value->getType();
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Simple assignment */
    if (variable->isType(StochasticNode_name) && value != NULL)
        ((StochasticNode*)(variable))->clamp(value);
    else {
        ConstantNode* newVar;
        if (value == NULL)
            newVar = new ConstantNode(variable->getType());
        else
            newVar = new ConstantNode(value);
        delete variable;
        variable = new ConstantNode(value);
    }
}


/** Set variable */
void Frame::setVariable(const std::string& name, RbObjectWrapper* var) {

    // Throw if var is NULL
    if (var == NULL)
        throw RbException("Invalid assignment: null wrapper");

    // Find the variable
    std::map<std::string, RbObjectWrapper*>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->setVariable(name, variable);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    // We are responsible for setting it
    RbObjectWrapper* variable = (*it).second;

    /* Check for repeated assignment first */
    if (variable->getDim() > 0 && var->getDim() == 0 && var->isValueType(variable->getValueType())) {

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
    if (variable->getDim() != var->getDim() || !var->isValueType(variable->getValueType())) {
        std::ostringstream msg;
        msg << variable->getTypeDescr() << " variable does not take ";
        msg << var->getTypeDescr();
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
        setValue(value);
 
    /* Find the variable */
    std::map<std::string, RbObjectWrapper*>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->setValElement(name, index, value);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    /* We are responsible for setting it */
    RbObjectWrapper* variable = (*it).second;

    /* Check dimension */
    if (int(index.size()) < variable->getDim() ||
        (int(index.size()) > variable->getDim() && !variable->isValueType(RbComplex_name)))
        throw (RbException("Subscript error"));

    /* Divide index in variable index and value index */
    IntVector variableIndex, valueIndex;
    for (size_t i=0; i<index.size(); i++) {
        if (i < variable->getDim())
            variableIndex.push_back(index[i]);
        else
            valueIndex.push_back(index[i]);
    }

    /* Check type */
    if (index.size() == variable->getDim() && value != NULL && !value->isType(variable->getValueType())) {
        std::stringstream msg;
        msg << variable->getTypeDescr() << " variable does not take ";
        msg << value->getType();
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Assignment */
    variable->setElement(index, val);
}


/** Set element of variable with DAG node */
void Frame::setVarElement(const std::string& name, const IntVector& index, DAGNode* var) {

     /* Check for inadvertent calls */
    if (index.size() == 0)
        setVariable(var);
 
    /* Find the variable */
    std::map<std::string, RbObjectWrapper*>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->setVarElement(name, index, variable);
        else
            throw (RbException("Variable " + name + " does not exist"));
    }

    /* We are responsible for setting it */
    RbObjectWrapper* variable = (*it).second;

    /* Check index */
    if (int(index.size()) i!= variable->getDim())
        throw (RbException("Subscript error"));

    /* Disallow container assignment */
    if (!var->isType(DAGNode_name))
        throw RbException("Invalid assignment of container to variable element");

    /* Check type */
    if (!variable->isValueType(var->getValueType())) {
        std::stringstream msg;
        msg << variable->getTypeDescr();
        msg << " variable does not take ";
        msg << var->getValueType();
        msg << " value";
        throw (RbException(msg.str()));
    }

    /* Assignment */
    variable->setElement(index, var);
}


/** Complete info abot object to string */
std::string Frame::toString(void) const {

    std::ostringstream o;
    o << "Frame:" << std::endl;
    printValue(o);

    return o.str();
}



