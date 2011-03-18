/**
 * @file
 * This file contains the implementation of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Implementation of VariableSlot
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

#include "DAGNode.h"
#include "Frame.h"
#include "RbException.h"
#include "RbNames.h"
#include "VariableSlot.h"

#include <cassert>


/** Constructor of filled slot. We get the type specification from the variable. */
VariableSlot::VariableSlot(DAGNode* var, bool ref)
: typeSpec(var->getType(), var->getDim(), ref), temp(false), variable(NULL), frame(NULL) {

    if (var->getSlot() == NULL && typeSpec.isReference())
        temp = true;

    if (typeSpec.isReference())
        setReference(var);
    else
        setValue(var);

}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const std::string& type, int dim, bool ref)
: typeSpec(type, dim, ref), temp(false), variable(NULL), frame(NULL) {
}


/** Copy constructor. We need to copy intelligently based on whether the value is a reference, a temp reference or a regular value.
    We do not copy frame because the copy is likely to live in  a separate frame. */
VariableSlot::VariableSlot(const VariableSlot& x) : typeSpec(x.getTypeSpec()), temp(false), variable(NULL), frame(NULL) {

    if (!typeSpec.isReference()) {
        if (x.variable != NULL) {
            variable = x.variable->clone();
            variable->setSlot(this);
        }
    }
    else /* if (typeSpec.isReference()) */ {
         variable = x.variable;
         if (variable != NULL) {
             if (variable->isType(LookupNode_name))
                 variable = variable->clone();
             variable->addReferringSlot(this);
         }
    }
}


/** Manage variable destruction differently based on whether the value is a reference, a temp reference or a regular value. */
VariableSlot::~VariableSlot(void) {

    removeVariable();
}


/** Assignment operator */
VariableSlot& VariableSlot::operator=(const VariableSlot& x) {
    
    if (&x != this) {

        // Check if assignment is possible
        if (typeSpec != x.typeSpec)
            throw RbException ("Invalid slot assignment: type difference");

        // Remove old variable
        removeVariable();
        
        // The frame of the slot does not copy over

        // Copy the new variable
        if (!typeSpec.isReference()) {
            if (x.variable != NULL) {
                variable = x.variable->clone();
                variable->setSlot(this);
            }
        }
        else /* if (typeSpec.isReference()) */ {
             variable = x.variable;
             if (variable != NULL) {
                 if (variable->isType(LookupNode_name))
                     variable = variable->clone();
                 variable->addReferringSlot(this);
             }
        }
    }

    return (*this);
}


/** Get name of the slot from frame */
const std::string& VariableSlot::getName(void) const {

    if (frame == NULL)
        return EmptyString;

    return frame->getSlotName(this);
}


/** Get a reference to the slot variable */
DAGNode* VariableSlot::getReference(void) {

    if (!typeSpec.isReference())
        throw RbException("Cannot get reference to variable in value slot");
    
    return variable->getReference();
}


/** Get the value of the variable */
const DAGNode* VariableSlot::getValue(void) const {

    /* The DAGNode function getReference returns the variable itself if not a lookup node, in which case
       the reference is the result of the lookup */
    return variable->getReference();
}


/* Print value of the slot variable */
void VariableSlot::printValue(std::ostream& o) const {

    if (variable == NULL)
        o << "NULL";
    else
        getValue()->printValue(o);
}


/** Remove variable differently based on whether the value is a reference, a temp reference or a regular value. */
void VariableSlot::removeVariable(void) {
    
    if (typeSpec.isReference() && temp == false) {
        // Indirect reference through lookup node; lookup node cannot have referring slots.
        if (variable->isType(LookupNode_name)) {
            variable->setSlot(NULL);
            delete variable;
        }
        else {
            // Direct reference; we do not own the variable. Simply remove the reference.
            variable->removeReferringSlot(this);
        }
    }
    else {
        // Regular variable or reference slot filled with a temp variable. We own the variable.
        if (variable != NULL) {
            if (variable->getReferringSlots().size() != 0) {
                // Other slots need the variable; let somebody else take care of it. The variable keeps its original name.
                VariableSlot* otherSlot = *variable->getReferringSlots().begin();
                otherSlot->temp = true;
                variable->getReferringSlots().erase(otherSlot);
                variable->setSlot(otherSlot);
            }
            else {
                // The variable goes if it does not have any children. If it has children, the children will manage it.
                variable->setSlot(NULL);
                if (variable->numChildren() == 0)
                    delete variable;
            }
        }
    }

    variable = NULL;
    temp = false;
}


/** Set a reference slot to a new variable reference */
void VariableSlot::setReference(DAGNode* ref) {

    if (!typeSpec.isReference())
        throw RbException("Cannot set value slot with a reference");

    removeVariable();

    variable = ref;
    if (variable != NULL) {
        if (variable->getSlot() == NULL) {
            if (variable->isType(LookupNode_name))
                temp = false;
            else
                temp = true;
            variable->setSlot(this);
        }
        else {
            temp = false;
            variable->addReferringSlot(this);
        }
    }
}


/** Set reference flag of slot */
void VariableSlot::setReferenceFlag(bool refFlag) {

    if (refFlag == true && typeSpec.isReference() == false) {
        // The new reference is a reference to a temp value
        typeSpec.setReference(true);
        temp = true;        
    }
    else if (refFlag == false && typeSpec.isReference() == true) {
        typeSpec.setReference(false);
        if (temp == true) {
            // The new value is the old value as a temp in a reference slot
            temp = false;
        }
        else {
            // Direct reference or indirect reference through lookup. The new value is a clone of the reference
            setValue(variable->getReference()->clone());
        }
    }
}


/** Set a value slot to a new variable value */
void VariableSlot::setValue(DAGNode* value) {

    assert(value->getSlot() == NULL && value->getReferringSlots().size() == 0 && value->getName() == "");

    removeVariable();

    if (typeSpec.isReference())
        temp = true;
    variable = value;
    variable->setSlot(this);
}


/** Set a value slot to a new variable value */
void VariableSlot::setValue(RbObject* value) {

    // Wrap the value appropriately and then set the slot
    if ( value->isType(MemberObject_name) )
        setVariable( new MemberNode( static_cast<MemberObject*>( value ) ) );
    else if ( value->isType( Container_name ) )
        setVariable( new DAGNodePlate( static_cast<Container*>( value ) ) );
    else
        setVariable( new ConstantNode( value ) );
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const TypeSpec& x) {

    o << "<" << typeSpec << ">";
    if ( getName() != "" )
        o << " " << getName();
    o << " =";
    if ( temp )
        o << " [temp]"
    if ( variable == NULL )
        o << " NULL";
    else {
        o << " ";
        variable->printValue(o);
    }
    return o;
}


#if 0
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

    // Get handy reference to slot
    VariableSlot& slot = (*it).second;

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
void VariableSlot::swapReference(DAGNode* oldRef, DAGNode* newRef) {

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

#endif
