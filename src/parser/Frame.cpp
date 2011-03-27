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
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#include "ContainerNode.h"
#include "Frame.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"      // For PRINTF
#include "VariableContainer.h"

#include <sstream>


/** Construct frame with NULL parent */
Frame::Frame(void) : parentFrame(NULL), variableTable() {
}


/** Construct frame with parent */
Frame::Frame(Frame* parentFr) : parentFrame(parentFr), variableTable() {
}


/** Copy constructor. We need to set the frame of the variable slots. */
Frame::Frame(const Frame& x) :  parentFrame(x.parentFrame), variableTable(x.variableTable) {

    for (VariableTable::iterator i=variableTable.begin(); i!=variableTable.end(); i++)
        (*i).second.setFrame(this);
}
    

/** Add reference variable to frame */
void Frame::addReference(const std::string& name, DAGNode* ref) {

    /* Throw an error if the ref is NULL */
    if (ref == NULL)
        throw RbException("Cannot insert reference variable in variable table based on NULL pointer");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw RbException("Variable " + name + " already exists");

    /* Create the slot */
    VariableSlot slot(ref, true);
    slot.setFrame(this);

    /* Insert new reference variable in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

    PRINTF("Inserted  variable reference named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), ref->getValueType().c_str(), ref->getDim());
}


/** Add declared but empty reference slot to frame */
void Frame::addReference(const std::string& name, const std::string& type, int dim) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    /* Create the slot */
    VariableSlot slot(type, dim, true);
    slot.setFrame(this);

    /* Insert empty slot in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

    PRINTF("Inserted null variable reference named '%s' of type '%s' and dim %d in frame\n",
        name.c_str(), type.c_str(), dim);
}


/** Add simple variable to frame */
void Frame::addVariable(const std::string& name, DAGNode* value) {

    /* Throw an error if the value is NULL */
    if (value == NULL)
        throw RbException("Cannot insert variable in variable table based on NULL value.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    /* Create the slot */
    VariableSlot slot(value);
    slot.setFrame(this);

    /* Insert new variable in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

    PRINTF("Inserted variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), value->getValueType().c_str(), value->getDim());
}


/** Add plate variable with initial element to frame */
void Frame::addVariable(const std::string& name, const VectorInteger& index, DAGNode* elemValue) {

    /* Throw an error if the variable is NULL */
    if (elemValue == NULL)
        throw RbException("Cannot insert null variable container without type in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    /* Create the container variable */
    VariableContainer* container = new VariableContainer(TypeSpec(elemValue->getValueType(), index.size()));
    container->setElement(index, elemValue);

    /* Create the slot */
    VariableSlot slot(new ContainerNode( container ));
    slot.setFrame(this);

    /* Insert new variable in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

    PRINTF("Inserted variable %s %s in frame\n",
            container->getTypeSpec(), name.c_str());
}


/** Add declared but empty variable slot to frame */
void Frame::addVariable(const std::string& name, const std::string& type, int dim) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames, which
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    /* Create the slot */
    VariableSlot slot(type, dim);
    slot.setFrame(this);

    /* Insert new variable in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

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

    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end())
        throw (RbException("Variable " + name + " does not exist"));

    variableTable.erase(it);

    PRINTF("Erased variable slot named '%s' in frame\n", name.c_str());
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


/** Get reference */
DAGNode* Frame::getReference(const std::string& name) const {

    PRINTF("Retrieving variable reference named '%s' from frame\n", name.c_str());

    std::map<std::string, VariableSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getReference(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second.getReference();
}


/** Get slot name */
const std::string& Frame::getSlotName(const VariableSlot* theSlot) const {

    for (VariableTable::const_iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
    
        if (&((*i).second) == theSlot)
            return (*i).first;
    }

    throw RbException("Specified variable slot does not exist in frame");
}


/** Get value */
const RbObject* Frame::getValue(const std::string& name) const {

    PRINTF("Retrieving value of variable named '%s' from frame\n", name.c_str());

    std::map<std::string, VariableSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getValue(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second.getValue();
}


/** Get variable slot */
VariableSlot& Frame::getVariableSlot(const std::string& name) {

    PRINTF("Retrieving variable slot named '%s' from frame\n", name.c_str());

    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariableSlot(name);
        else
            throw (RbException("Variable slot '" + name + "' does not exist"));
    }

    return (*it).second;
}


/** Is the named variable a reference variable? */
bool Frame::isReference(const std::string& name) const {

    std::map<std::string, VariableSlot>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->isReference(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second.getTypeSpec().isReference();
}


/** Print value for user */
void Frame::printValue(std::ostream& o) const {

    VariableTable::const_iterator i;
    for (i=variableTable.begin(); i!=variableTable.end(); i++) {
        o << (*i).first << " = ";
        (*i).second.printValue(o);
        o << std::endl;
    }
}


/** Complete info about object to string */
std::string Frame::richInfo(void) const {

    std::ostringstream o;
    o << "Frame:" << std::endl;
    printValue(o);

    return o.str();
}


/** Set value */
void Frame::setValue(const std::string& name, RbObject* newVal) {

    // Find the variable
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setValue(name, newVal);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // Delegate to slot
    VariableSlot& slot = (*it).second;
    slot.setValue(newVal);
}


/** Set variable */
void Frame::setVariable(const std::string& name, DAGNode* newVar) {

    // Find the variable slot
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setVariable(name, newVar);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // Delegate to slot
    VariableSlot& slot = (*it).second;
    slot.setVariable(newVar);
}


/** Set reference */
void Frame::setReference(const std::string& name, DAGNode* newRef) {

    // Find the variable slot
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setReference(name, newRef);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // Delegate to slot
    VariableSlot& slot = (*it).second;
    slot.setReference(newRef);
}


/** Swap reference to be a reference to another variable */
void Frame::swapReference(DAGNode* oldRef, DAGNode* newRef) {

    // Find the variable (it is inefficient but this function is not used during the mcmc)
    std::map<std::string, VariableSlot>::iterator i;
    for ( i = variableTable.begin(); i != variableTable.end(); i++ ) {
        if ( (*i).second.variable == oldRef )
            break;
    }
    if ( i == variableTable.end() ) { 
        throw ( RbException( "Variable reference does not exist in this frame" ) );
    }

    (*i).second.setReference( newRef );
}


