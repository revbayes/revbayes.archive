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

#include "DAGNodePlate.h"
#include "Frame.h"
#include "VariableSlot.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbOptions.h"      // For PRINTF

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

    /* Create the plate variable */
    VectorInteger length = index;
    for (size_t i=0; i<length.size(); i++)
        length[i]++;
    DAGNodePlate* plate = new DAGNodePlate(length, elemValue->getValueType());
    plate->setElement(index, elemValue);

    /* Create the slot */
    VariableSlot slot(plate);
    slot.setFrame(this);

    /* Insert new variable in variable table */
    variableTable.insert(std::pair<std::string, VariableSlot>(name, slot));

    PRINTF("Inserted variable named '%s' of type '%s' and dim %d in frame\n",
            name.c_str(), plate->getValueType().c_str(), plate->getDim());
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
DAGNode* Frame::getReference(const std::string& name) {

    PRINTF("Retrieving variable reference named '%s' from frame\n", name.c_str());

    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
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
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    return (*it).second;
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
void Frame::setValue(const std::string& name, RbObject* value) {

    // Find the variable
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setValue(name, value);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // Delegate to slot
    VariableSlot& slot = (*it).second;
    slot.setValue(newVal);
}


    // Delegate to the owner if a reference variable
    if ( slot.reference == true && slot.temp == false && slot.variable != NULL ) {
        slot.variable->getFrame()->setValue(slot.variable->getName(), value);
        return;
    }

    // Note that we let the case of a reference variable, which is NULL or temp, being set by a
    // value pass through. There is no owner frame to delegate to in this case, so use the
    // standard code to deal with this case and set temp to true to indicate that the reference
    // slot holds a temporary variable managed by the frame.

    // Do type conversion if necessary
    bool mutate = false;
    if ( value != NULL ) {
        bool typeMatch;
        if (value->isType(Container_name)) {
            Container* container = (Container*)(value);
            typeMatch = Workspace::userWorkspace().isXOfTypeY(container->getElementType(), slot.type) && container->getDim() == slot.dim;
        }
        else {
            typeMatch = Workspace::userWorkspace().isXOfTypeY(value->getType(), slot.type) && slot.dim == 0;
        }
        if (typeMatch == false) {
            if ( value->isConvertibleTo(slot.type, slot.dim) ) {
                // We are golden, just convert to the right value type and delete old value
                RbObject* oldValue = value;
                value = value->convertTo(slot.type, slot.dim);
                delete oldValue;    // We are responsible for freeing the memory
            }
            else if ( slot.variable->isMutableTo(VectorInteger(), value) ) {
                // We set a flag that triggers mutation below
                mutate = true;
            }
            else {
                // No possibilities left
                std::ostringstream msg;
                msg << slot.type;
                for (int i=0; i<slot.dim; i++)
                    msg << "[]";
                if (slot.reference == true)
                    msg << "&";
                msg << " variable does not take ";
                msg << value->getType();
                if (value->isType(Container_name)) {
                    for (int i=0; i<value->getDim(); i++)
                        msg << "[]";
                }
                msg << " value";
                throw(RbException(msg.str()));
		    }
        }
    }
	
    /* Set value of stochastic node */
    if (value != NULL && slot.variable != NULL && slot.variable->isType(StochasticNode_name)) {
        ((StochasticNode*)(slot.variable))->setValue(value);
        return;
	}

    /* Create new variable */
    DAGNode* variable;
    if (value == NULL)
        variable = new ConstantNode(slot.type);
    else if (value->isType(Container_name))
        variable = new DAGNodePlate((Container*)(value));
    else
        variable = new ConstantNode(value);

    // Update DAG and referring frames if oldVariable is not NULL
    if ( slot.variable != NULL ) {

        // Update DAG and referring frames
        if (mutate == true)
            slot.variable->mutateTo(variable);
        else
            slot.variable->swapNodeTo(variable);
        
        /* Remove old variable */
        removeOldVariable(slot);
	}

    /* Find name of new variable */
    std::string varName;
    if (frameName != "")
        varName = frameName + "." + name;
    else
        varName = name;

    /* Set new variable containing new value */
    if ( slot.reference == true )
        slot.temp = true;
    variable->setName(varName);
    variable->setFrame(this);
    slot.variable = variable;
}


/** Set variable */
void Frame::setVariable(const std::string& name, DAGNode* variable) {

    // Find the variable slot
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            parentFrame->setVariable(name, variable);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    // Get handy reference to slot
    VariableSlot& slot = (*it).second;

    // Special case if new variable is NULL
    if (variable == NULL) {
        if (slot.variable != NULL)
            removeOldVariable(slot);
        slot.variable = NULL;
        return;
	}
	
    // Set a reference variable
    if ( slot.reference == true ) {
        if ( variable->getFrame() != NULL ) {
            // We actually have been given a reference. We do not allow any type conversion in this case
            if ( variable->getDim() == slot.dim && Workspace::globalWorkspace().isXOfTypeY(variable->getValueType(), slot.type) ) {
                removeOldVariable(slot);
                slot.variable = variable;
                slot.temp = false;
                slot.variable->addReferringFrame(this);
                return;
            }
            else
                throw RbException("Wrong type for setting variable reference");
        }
        else if ( variable->getFrame() == NULL && slot.variable != NULL && slot.temp == false ) {
            // We are trying to set a value through a reference. Delegate to owner frame.
            slot.variable->getFrame()->setVariable(name, variable);
            return;
        }
    }
    
    // Note that we let the case of a reference variable, which is NULL or temp, being set by a
    // non-reference DAG node (temp variable) pass through. There is no owner frame to delegate
    // to in this case, so use the standard code to deal with this case and set temp to true to
    // indicate that the reference slot holds a temporary variable managed by the frame.

    // If conversion of constant to stochastic node, save value of old variable
    if (slot.variable != NULL &&
        slot.variable->isType(ConstantNode_name) &&
        slot.variable->getValue() != NULL &&
        variable->isType(StochasticNode_name)) {
        
        if ( slot.variable->getValue()->isType(variable->getValueType()) ) {
            ((StochasticNode *)variable)->setValue(slot.variable->getValue()->clone());
        }
        else if ( slot.variable->getValue()->isConvertibleTo(variable->getValueType()) ) {
            ((StochasticNode *)variable)->setValue(slot.variable->getValue()->convertTo(variable->getValueType()));   
        }
    }

    // Do type conversion if necessary
    bool mutate = false;
    if ( !Workspace::userWorkspace().isXOfTypeY(variable->getValueType(), slot.type) || variable->getDim() != slot.dim ) {
        if ( Workspace::userWorkspace().isXConvertibleToY(variable->getValueType(), variable->getDim(), slot.type, slot.dim) ) {
            // We are golden, just insert a converter node to the right value type
            ConverterNode* convNode = new ConverterNode(variable, slot.type, slot.dim);
            variable = convNode;
		}
        else if ( slot.variable != NULL && slot.variable->isMutableTo(variable) ) {
            // Set a flag that triggers mutation of the variable below
            mutate = true;
        }
        else {
            // No possibilities left
            std::ostringstream msg;
            msg << slot.type;
            for (int i=0; i<slot.dim; i++)
                msg << "[]";
            if (slot.reference == true)
                msg << "&";
            msg << " variable does not take ";
            msg << variable->getValueType();
            for (int i=0; i<variable->getDim(); i++)
                msg << "[]";
            if (variable->getFrame() != NULL)
                msg << "&";
            msg << " value";
            throw(RbException(msg.str()));
		}
    }

    // Update DAG and referring frames if old variable is not NULL; delete old variable
    if ( slot.variable != NULL ) {

        // Update DAG and referring frames
        if (mutate == true)
            slot.variable->mutateTo(variable);
        else
            slot.variable->swapNodeTo(variable);
        
        /* Remove old variable */
        removeOldVariable(slot);
	}

    /* Find name of new variable */
    std::string varName;
    if (frameName != "")
        varName = frameName + "." + name;
    else
        varName = name;

    /* Set new variable */
    if ( slot.reference == true )
        slot.temp = true;
    variable->setName(varName);
    variable->setFrame(this);
    slot.variable = variable;
}


/** Set element of variable with object */
void Frame::setValElement(const std::string& name, const VectorInteger& index, RbObject* value) {

    /* Check for inadvertent calls */
    if (index.size() == 0)
        setValue(name, value);
 
    /* Find the variable */
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
    if (it == variableTable.end()) {
        if (parentFrame != NULL)
            return parentFrame->setValElement(name, index, value);
        else
            throw (RbException("Variable '" + name + "' does not exist"));
    }

    /* Get handy reference to slot */
    VariableSlot& slot = (*it).second;

    /* Special case when slot.variable is NULL */
    VectorInteger length = index;
    for (size_t i=0; i<length.size(); i++)
        length[i]++;
    if (slot.variable ==  NULL) {
        if ((value == NULL || value->isType(slot.type)) && int(index.size()) == slot.dim) {
            DAGNodePlate* plate = new DAGNodePlate(length, slot.type);
            if (value != NULL)
                plate->setElement(index, value);
            slot.variable = plate;
            return;
        }
    }

    /* Check dimension */
    if (int(index.size()) < slot.variable->getDim() ||
        (int(index.size()) > slot.variable->getDim() && !Workspace::userWorkspace().isXOfTypeY(slot.variable->getValueType(), RbComplex_name)))
        throw (RbException("Subscript error"));

    /* Element assignment: variable checks types */
    slot.variable->setElement(index, value);
}


/** Set element of variable with DAG node */
void Frame::setVarElement(const std::string& name, const VectorInteger& index, DAGNode* var) {

     /* Check for inadvertent calls */
    if (index.size() == 0)
        setVariable(name, var);
 
    /* Find the variable */
    std::map<std::string, VariableSlot>::iterator it = variableTable.find(name);
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
            variable = (*it).second.variable = new DAGNodePlate(index, (*it).second.type);
        else
            variable = (*it).second.variable = new DAGNodePlate(index, var);
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
