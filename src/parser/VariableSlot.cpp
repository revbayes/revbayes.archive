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

#include "ConstantNode.h"
#include "Container.h"
#include "DAGNode.h"
#include "DAGNodePlate.h"
#include "Frame.h"
#include "LookupNode.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VariableSlot.h"

#include <cassert>


/** Constructor of filled slot. We get the type specification from the variable. */
VariableSlot::VariableSlot(DAGNode* var, bool ref)
    : typeSpec(var->getValueType(), var->getDim(), ref), temp(false), variable(NULL), frame(NULL) {

    if (var->getSlot() == NULL && typeSpec.isReference())
        temp = true;

    if (typeSpec.isReference())
        setReference(var);
    else
        setVariable(var);

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
             if (variable->isDAGType(LookupNode_name))
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
                 if (variable->isDAGType(LookupNode_name))
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
    
    return variable;
}


/** Get the value of the variable */
const RbObject* VariableSlot::getValue(void) const {

    return variable->getValue();
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
        if (variable->isDAGType(LookupNode_name)) {
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
            if (variable->isDAGType(LookupNode_name))
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
            setVariable(static_cast<LookupNode*>(variable)->getVariable());
        }
    }
}


/** Set a value slot to a new variable value */
void VariableSlot::setVariable(DAGNode* newVar) {

    assert(newVar->getSlot() == NULL && newVar->getReferringSlots().size() == 0 && newVar->getName() == "");

    removeVariable();

    if (typeSpec.isReference())
        temp = true;
    variable = newVar;
    variable->setSlot(this);
}


/** Set a slot to a new value */
void VariableSlot::setValue(RbObject* value) {

    // Delegate to the owner if a reference variable
    if ( typeSpec.isReference() && !temp && variable != NULL ) {
        variable->getSlot()->setValue( value );
        return;
    }

    // Note that we let the case of a reference variable, which is NULL or temp, being set by a
    // value pass through. There is no owner to delegate to in this case, so use the standard
    // code to deal with this case and set temp to true to indicate that the reference slot
    // holds a temporary variable managed by the slot.

    // Do type conversion if necessary
    bool mutate = false;
    if ( value != NULL ) {
        bool typeMatch;
        if ( value->isType( Container_name ) ) {
            Container* container = (Container*)( value );
            typeMatch = Workspace::userWorkspace().isXOfTypeY( container->getElementType(), typeSpec.getType() ) && container->getDim() == typeSpec.getDim();
        }
        else {
            typeMatch = Workspace::userWorkspace().isXOfTypeY( value->getType(), slot.type ) && slot.dim == 0;
        }
        if ( typeMatch == false ) {
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


    removeVariable();

    // Wrap the value appropriately and then set the slot
    if ( value->isType( MemberObject_name ) )
        setVariable( new MemberNode( dynamic_cast<MemberObject*>( value ) ) );
    else if ( value->isType( Container_name ) )
        setVariable( new DAGNodePlate( dynamic_cast<Container*>( value ) ) );
    else
        setVariable( new ConstantNode( value ) );
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const VariableSlot& x) {

    o << "<" << x.getTypeSpec() << ">";
    if ( x.getName() != "" )
        o << " " << x.getName();
    o << " =";
    if ( x.isTemp() )
        o << " [temp]";
    if ( x.getVariable() == NULL )
        o << " NULL";
    else {
        o << " ";
        x.getVariable()->printValue(o);
    }
    return o;
}

