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
#include "ContainerIterator.h"
#include "DAGNodeContainer.h"
#include "Frame.h"
#include "IntVector.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"         // For PRINTF
#include "StochasticNode.h"
#include "StringVector.h"
#include "Workspace.h"

#include <sstream>


/** Construct frame with NULL parent */
Frame::Frame(void) :
    parentFrame(NULL), variableTable(), frameName() {
}


/** Construct frame with parent */
Frame::Frame(Frame* parentFr) :
    parentFrame(parentFr), variableTable(), frameName() {
}


/** Copy constructor */
Frame::Frame(const Frame& x) :
    parentFrame(x.parentFrame), variableTable(x.variableTable), frameName(x.frameName) {

    for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
        std::string varName;
        if (frameName == "")
            varName = (*i).first;
        else
            varName = frameName + "." + (*i).first;
        if ((*i).second.reference == false && (*i).second.variable != NULL) {
            (*i).second.variable = (*i).second.variable->clone();
            (*i).second.variable->setName(varName);
        }
        /* Save temporaries (temp of slot already set to true) */
        else if ((*i).second.reference == true && (*i).second.variable != NULL && (*i).second.temp == true) {
            (*i).second.variable = (*i).second.variable->clone();
            (*i).second.variable->setName(varName);
        }
    }
}


/** Destructor */
Frame::~Frame(void) {

    size_t numVars;
    do {
        numVars = variableTable.size();
        for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
            if ((*i).second.variable == NULL) {
                variableTable.erase(i);
                break;
            }
            else if ((*i).second.reference == false && (*i).second.variable != NULL) {
                (*i).second.variable->setName("");
                if ((*i).second.variable->numRefs() == 0)
                    delete (*i).second.variable;
                variableTable.erase(i);
                break;
            }
            /* Delete temporaries */
            else if ((*i).second.reference == true && (*i).second.variable != NULL && (*i).second.temp == true) {
                (*i).second.variable->setName("");
                if ((*i).second.variable->numRefs() == 0)
                    delete (*i).second.variable;
                variableTable.erase(i);
                break;
            }
            else if ((*i).second.reference == true) {
                variableTable.erase(i);
                break;
            }
        }
    } while (numVars != variableTable.size());

    if (variableTable.size() != 0)
        throw RbException("Could not delete all variables in frame");
}


/** Assignment operator */
Frame& Frame::operator=(const Frame& x) {

    if (this != &x) {

        for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
            if ((*i).second.reference == false && (*i).second.variable != NULL) {
                (*i).second.variable->setName("");
                delete (*i).second.variable;
            }
            /* Delete temporaries */
            else if ((*i).second.reference == true && (*i).second.variable != NULL && (*i).second.temp == true) {
                (*i).second.variable->setName("");
                delete (*i).second.variable;
            }
        }

        variableTable = x.variableTable;
        frameName     = x.frameName;
        for (std::map<std::string, ObjectSlot>::iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
            std::string varName;
            if (frameName == "")
                varName = (*i).first;
            else
                varName = frameName + "." + (*i).first;
            if ((*i).second.reference == false && (*i).second.variable != NULL) {
                (*i).second.variable = (*i).second.variable->clone();
                (*i).second.variable->setName(varName);
            }
            /* Save temporaries */
            else if ((*i).second.reference == true && (*i).second.variable != NULL && (*i).second.temp == true) {
                (*i).second.variable = (*i).second.variable->clone();
                (*i).second.variable->setName(varName);
            }
        }
    }

    return (*this);
}


/** Equals operator */
bool Frame::operator==(const Frame& x) const {

    if (this != &x) {

        if (variableTable.size() != x.variableTable.size())
            return false;

        VariableTable::const_iterator i, j;
        for (i=variableTable.begin(), j=x.variableTable.begin(); i!=variableTable.end(); i++, j++) {
            if ((*i).first != (*j).first)
                return false;
            if ((*i).second.dim != (*j).second.dim)
                return false;
            if ((*i).second.type != (*j).second.type)
                return false;
            if ((*i).second.reference != (*j).second.reference)
                return false;
            if ((*i).second.variable != (*j).second.variable)
                return false;
        }
    }

    return true;
}


/** Not equals operator */
bool Frame::operator!=(const Frame& x) const {

    return !operator==(x);
}


/** Add reference variable to frame */
void Frame::addReference(const std::string& name, DAGNode* varPtr) {

    /* Throw an error if the varPtr is NULL */
    if (varPtr == NULL)
        throw RbException("Cannot insert reference variable pointing to NULL in variable table.");

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot;
    slot.type      = varPtr->getValueType();
    slot.dim       = 0;
    slot.reference = true;
    if (varPtr->getName() != "")
        slot.temp      = false;
    else
        slot.temp      = true;
    slot.variable  = varPtr;

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted  variable reference named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), slot.variable->getValueType().c_str(), slot.variable->getDim());
}


/** Add declared but empty reference slot to table */
void Frame::addReference(const std::string& name, const std::string& type, int dim) {

    /* Throw an error if the variable exists. Note that we cannot use the function
       existsVariable because that function looks recursively in parent frames. This
       would make it impossible to hide global variables. */
    if (variableTable.find(name) != variableTable.end())
        throw (RbException("Variable " + name + " already exists"));

    ObjectSlot slot;
    slot.type      = type;
    slot.dim       = dim;
    slot.reference = true;
    slot.temp      = false;
    slot.variable  = NULL;

    variableTable.insert(std::pair<std::string, ObjectSlot>(name, slot));

    PRINTF("Inserted null variable reference named '%s' of type '%s' and dim %d in frame\n",
        name.c_str(), type.c_str(), dim);
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
    slot.type      = value->getType();
    slot.dim       = 0;
    slot.reference = false;
    slot.temp      = false;
    slot.variable  = new ConstantNode(value);
    slot.variable->setName(name);

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
    slot.type      = var->getValueType();
    slot.dim       = var->getDim();
    slot.reference = false;
    slot.temp      = false;
    slot.variable  = var;
    slot.variable->setName(name);

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

    IntVector length = index;
    for (size_t i=0; i<length.size(); i++)
        length[i]++;
    DAGNodeContainer* container = new DAGNodeContainer(length, var->getValueType());
    container->setElement(index, var);

    ObjectSlot slot;
    slot.type      = container->getValueType();
    slot.dim       = container->getDim();
    slot.reference = false;
    slot.temp      = false;
    slot.variable  = container;
    slot.variable->setName(name);

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
    slot.type      = type;
    slot.dim       = dim;
    slot.reference = false;
    slot.temp      = false;
    slot.variable  = NULL;

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

    if ((*it).second.reference == false && (*it).second.variable != NULL)
        delete (*it).second.variable;
    else if ((*it).second.reference == true && (*it).second.variable != NULL && (*it).second.variable->getName() == "")
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


/** Does variable exist in the frame (excluding encapsulating frames)? */
const Frame* Frame::getVariableFrame(const std::string& name) const {

    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariableFrame(name);
        else
            return NULL;
    }

    return this;
}


/** Get dim of variable slot */
int Frame::getDim(const std::string& name) const {

    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getDim(name);
        else
            throw RbException("Variable '" + name + "' does not exist");
    }

    return (*it).second.dim;
}


/** Get type of variable slot */
const std::string& Frame::getType(const std::string& name) const {

    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getType(name);
        else
            throw RbException("Variable '" + name + "' does not exist");
    }

    return (*it).second.type;
}


/** Get value (read-only) */
const RbObject* Frame::getValue(const std::string& name) {

    PRINTF("Retrieving value of variable named '%s' from frame\n", name.c_str());
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getValue(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second.variable->getValue();
}


/** Get variable (read-only) */
const DAGNode* Frame::getVariable(const std::string& name) const {

    PRINTF("Retrieving variable named '%s' from frame\n", name.c_str());
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second.variable;
}


/** Get variable (non-const) */
DAGNode* Frame::getVariable(const std::string& name) {

    PRINTF("Retrieving non-const variable named '%s' from frame\n", name.c_str());
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (variableTable.find(name) == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVariable(name);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
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
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        return (*it).second.variable->getValue();

    return (*it).second.variable->getValElement(index);
}


/** Get variable element (non-const) */
DAGNode* Frame::getVarElement(const std::string& name, const IntVector& index) {

    // Find the variable
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVarElement(name, index);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // We are responsible for getting it
    if (index.size() == 0)
        getVariable(name);

    if (int(index.size()) != (*it).second.dim)
        throw (RbException("Subscript error"));

    return ((DAGNodeContainer*)((*it).second.variable))->getVarElement(index);
}


/** Get variable element (read-only) */
const DAGNode* Frame::getVarElement(const std::string& name, const IntVector& index) const {

    // Find the variable
    std::map<std::string, ObjectSlot>::const_iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->getVarElement(name, index);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
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


/** Set base name of frame and modify all variable names */
void Frame::setFrameName(const std::string& name) {

    frameName = name;
    for (VariableTable::iterator i=variableTable.begin(); i!=variableTable.end(); i++) {
        if (!((*i).second.reference == true && (*i).second.temp == false)) {
            std::string varName = frameName + "." + (*i).first;
            (*i).second.variable->setName(varName);
        }
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
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // We are responsible for setting it
    DAGNode* variable = (*it).second.variable;
    bool referenceVar = (*it).second.reference;

    /* Check type */
    if (value != NULL && !Workspace::userWorkspace().isXOfTypeY(value->getType(), (*it).second.type)) 
		{
        if ( value->isConvertible((*it).second.type) ) 
			{
            value = value->convert((*it).second.type);
			}
        else 
			{
            std::ostringstream msg;
            msg << (*it).second.type;
            msg << " variable does not take ";
            msg << value->getType();
            msg << " value";
            throw(RbException(msg.str()));
			}
		}
	
    /* Set value of stochastic node */
    if (variable != NULL && value != NULL && variable->isType(StochasticNode_name)) 
		{
        ((StochasticNode*)(variable))->setValue(value);
        return;
		}

    /* Create new variable */
    ConstantNode* var;
    if (value == NULL)
        var = NULL;
    else
        var = new ConstantNode(value);

    /* Update DAG if necessary */
    if (variable != NULL && referenceVar == false) 
		{
        if (variable->numChildren() > 0 && var == NULL)
            throw RbException("Cannot set node with children to NULL");

        /* It is important that children here is a copy, because swapping parents will affect children */
        std::set<VariableNode*> children = variable->getChildren();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->swapParentNode(variable, var);

        variable->setName("");
            delete variable;
		}

    /* Set new variable */
    if (referenceVar == false)
        var->setName(name);
    (*it).second.variable = var;
}


/** Set variable */
void Frame::setVariable(const std::string& name, DAGNode* var) {

    /* Find the variable */
    std::map<std::string, ObjectSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) 
		{
        if (parentFrame != NULL) 
			{
            parentFrame->setVariable(name, var);
            return;
			}
        else
            throw (RbException("Variable '" + name + "' does not exist"));
		}

    // We are responsible for setting it
    DAGNode* variable = (*it).second.variable;
    bool referenceVar = (*it).second.reference;

    // Special case if var is NULL
    if (var == NULL) 
		{
        if (variable != NULL) 
			{
            if (referenceVar == false || (referenceVar == true && (*it).second.temp == true)) 
				{
                if (variable->numChildren() != 0)
                    throw RbException("Cannot set node with children to NULL");
                variable->setName("");
                if (variable->numRefs() == 0)
                    delete variable;
				}
			}
        (*it).second.variable = NULL;
        return;
		}

    /* Check type and dim */
    if ( !Workspace::userWorkspace().isXOfTypeY(var->getTypeOfValue(), (*it).second.type) ) 
		{
        // TODO need to try to convert the variable
        std::cout << "Try to convert " << var->getTypeOfValue() << " into " << (*it).second.type << std::endl;
        std::cout << "Var is of type " << var->getClass()[0] << std::endl;
        var->printStruct(std::cout);
        if (var->isValueConvertibleTo((*it).second.type)) 
			{
            var->convertValueTo((*it).second.type);
			}
        else 
			{
            std::ostringstream msg;
            msg << (*it).second.type;
            msg << " variable does not take ";
            msg << var->getTypeOfValue();
            msg << " value";
            throw(RbException(msg.str()));
			}
		}
	
	/*if ((*it).second.dim != var->getDim() || !Workspace::userWorkspace().isXOfTypeY(var->getValueType(), (*it).second.type)) 
		{
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
		}*/

    /* Simple assignment */

    /* Update DAG if necessary */
    if (variable != NULL && (referenceVar == false || (*it).second.temp == true)) {

        std::set<VariableNode*> children = variable->getChildren();
        for (std::set<VariableNode*>::iterator i=children.begin(); i!=children.end(); i++)
            (*i)->swapParentNode(variable, var);

        variable->setName("");
            delete variable;
        (*it).second.temp = false;
    }

    /* Set new variable */
    std::string varName;
    if (frameName != "")
        varName = frameName + "." + name;
    else
        varName = name;
    if (referenceVar == false) {
        var->setName(varName);
    }
    else if (referenceVar == true && var->getName() == "") {
        // Save temporary variable here
        var->setName(varName);
        (*it).second.temp = true;
    }
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
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    /* We are responsible for setting it */
    DAGNode* variable = (*it).second.variable;

    /* Special case when variable is NULL */
    IntVector length = index;
    for (size_t i=0; i<length.size(); i++)
        length[i]++;
    if ((value == NULL || value->isType((*it).second.type)) && int(index.size()) == (*it).second.dim && variable == NULL) {
        if (value == NULL)
            variable = (*it).second.variable = new DAGNodeContainer(length, (*it).second.type);
        else
            variable = (*it).second.variable = new DAGNodeContainer(length, new ConstantNode(value));
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
        int(index.size()) == (*it).second.dim && variable == NULL) {
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

