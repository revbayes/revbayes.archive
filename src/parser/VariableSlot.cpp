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
#include "ContainerNode.h"
#include "Frame.h"
#include "LookupNode.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VariableSlot.h"
#include "Workspace.h"

#include <cassert>


/** Constructor of filled slot. We get the type specification from the variable. */
VariableSlot::VariableSlot(DAGNode* var, bool ref)
    : typeSpec(var->getValueType(), var->getDim(), ref), variable(NULL), frame(NULL) {

    if (typeSpec.isReference())
        setReference(var);
    else
        setVariable(var);

}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const std::string& type, int dim, bool ref)
    : typeSpec(type, dim, ref), variable(NULL), frame(NULL) {
}


/** Copy constructor. We need to copy intelligently based on whether the value is a reference, a temp reference or a regular value.
    We do not copy frame because the copy is likely to be inserted into a different frame (or none at all). */
VariableSlot::VariableSlot(const VariableSlot& x) : typeSpec(x.getTypeSpec()), variable(NULL), frame(NULL) {

    if ( !typeSpec.isReference() ) {
        if (x.variable != NULL) {
            variable = x.variable->clone();
            variable->setSlot( this );
        }
    }
    else /* if ( typeSpec.isReference() ) */ {
         variable = x.variable;
         if ( variable != NULL ) {
             variable->addReferringSlot( this );
         }
    }
}


/** Call a help function to remove the variable intelligently */
VariableSlot::~VariableSlot(void) {

    removeVariable();
}


/** Assignment operator */
VariableSlot& VariableSlot::operator=(const VariableSlot& x) {
    
    if ( &x != this ) {

        // Check if assignment is possible
        if ( typeSpec != x.typeSpec )
            throw RbException ( "Invalid slot assignment: type difference" );

        // Remove old variable
        removeVariable();
        
        // The frame of the slot does not copy over

        // Copy the new variable
        if ( !typeSpec.isReference() ) {
            if ( x.variable != NULL ) {
                variable = x.variable->clone();
                variable->setSlot( this );
            }
        }
        else /* if (typeSpec.isReference()) */ {
             variable = x.variable;
             if ( variable != NULL ) {
                 variable->addReferringSlot(this);
             }
        }
    }

    return (*this);
}


#if 0   
/** Convert a new candidate variable so that it fits in the slot, if possible */
DAGNode* VariableSlot::convertVariable( DAGNode* newVariable ) const {

    // We can always set slot to a NULL variable using new ConstantNode(getType())
    if ( newVariable == NULL )
        return newVariable;

    // Get current value of new variable
    const RbObject* newValue = newVariable->getValue();

    if ( !isReference() || newVariable->isTemp() ) {
    
        // Only current value of interest

        // If we are going to use it for a reference slot, it had better be a constant node
        if ( isReference() && !newVariable->isDAGType( ConstantNode_name ) )
            throw( "Invalid assignment of a non-const temp variable to a reference slot" );

        if ( newValue == NULL )
            return newVariable;

        if ( newValue->isTypeSpec( typeSpec ) )
            return newVariable;

        // Note: If we are setting a dag expression slot with a reference, the
        // statements below will convert the dag expression to a constant value
        // or copy the value unnecessarily. Caller needs to keep this in mind
        if ( newValue->isConvertibleTo( typeSpec, true ) ) {
            RbObject* temp = newValue->convertTo( typeSpec );
            delete newVariable;
            if ( temp->isType( MemberObject_name ) )
                return new MemberNode( static_cast<MemberObject*>( temp ) );
            else if ( temp->isType( Container_name ) )
                return new ContainerNode( static_cast<Container*>( temp ) );
            else
                return new ConstantNode( temp );
        }

        if ( variable->isMutableTo( newVariable ) ) {
            variable->mutateTo( newVariable );
            return newVariable;
        }

        std::ostringstream msg;
        msg << "Invalid assignment to " << typeSpec << " " << getName();
        throw RbException( msg );
    }
    else {
    
        // We have a reference slot and newVariable is indeed a reference

        // The reference slot is going to use the reference so it is
        // important that the reference value is guaranteed to be the
        // right type. We cannot rely on current value for type testing,
        // in fact, newValue may well be NULL. Instead, we use the value
        // type specification of the newVariable and the Workspace to
        // decide on the validity of the assignment
        if ( Workspace::userWorkspace().isXOfTypeY( newVariable->getTypeSpec(), typeSpec ) )
            return newVariable;

        if ( newVariable->isMutableTo( typeSpec ) ) {
            DAGNode* temp = newVariable->mutateTo( typeSpec );
            if ( newVariable->numRefs() == 0 )
                delete newVariable;
            return temp;
        }

        if ( newVariable->numRefs() == 0 )
            delete newVariable;

        std::ostringstream msg;
        msg << "Invalid assignment to " << typeSpec << " " << getName();
        throw RbException( msg );
    }
}
#endif


/** Get name of the slot from frame */
const std::string& VariableSlot::getName(void) const {

    if (frame == NULL)
        return EmptyString;

    return frame->getSlotName( this );
}


/**
 * Get variable for parser during evaluation of the seman-
 * tic value of an expression. Normally, the variable slot
 * should not give out a variable for others to manage.
 * However, the parser can handle temp variables and
 * needs assistance from the slot in interpreting how
 * a variable should be accessed.
 *
 * The basic logic is to issue a temp if the slot is a
 * value slot or a reference slot set to hold a temp
 * value. Otherwise, we issue a reference to the current
 * variable. By calling the variable's function getReference()
 * we ensure that a lookup node or function node will return
 * the reference and not the lookup node or function node.
 */
DAGNode* VariableSlot::getParserVariable( void ) {

    if ( !isReference() )
        return variable->clone();
    else {
        if ( variable->getSlot() == this )
            return variable->clone();
        else
            return variable->getReference();
    }
}


/**
 * Get variable for parser during construction of a DAG,
 * that is, during the evaluation of an equation expression.
 *
 * The basic logic here is almost reverse to that of the twin
 * function getParserVariable. If we have a value slot, then
 * we would like to get a reference to the variable itself.
 * If we have a reference value and it points to a temp inside
 * the slot, then we want a temp constant value instead. If the
 * slot is a reference slot and references a regular variable,
 * then we just return the reference.
 */
DAGNode* VariableSlot::getParserVariableRef( void ) {

    if ( !isReference() )
        return variable;     // Should not be a lookup or a function returning a reference
    else {
        if ( variable->getSlot() == this )
            return variable->clone();
        else
            return variable->getReference();
    }
}


/** Get a reference to the slot variable */
DAGNode* VariableSlot::getReference(void) const {

    if ( !typeSpec.isReference() )
        throw RbException( "Cannot get reference to variable in value slot" );
    
    return variable->getReference();
}


/** Is variable valid for the slot? */
bool VariableSlot::isValidVariable( DAGNode* newVariable ) const {

    if ( newVariable == NULL )
        throw RbException( "Attempt to set slot " + getName() + " using invalid NULL variable" );

    // Get current value of new variable
    const RbObject* newValue = newVariable->getValue();

    if ( !isReference() || newVariable->isTemp() ) {
    
        // Only current value of interest

        // If we are going to use it for a reference slot, it had better be a constant node
        if ( isReference() && !newVariable->isDAGType( ConstantNode_name ) )
            return false;

        if ( newValue == NULL )
            return true;

        if ( newValue->isTypeSpec( typeSpec ) )
            return true;

        return false;
    }
    else {
    
        // We have a reference slot and newVariable is indeed a reference

        // The reference slot is going to use the reference so it is
        // important that the reference value is guaranteed to be the
        // right type. We cannot rely on current value for type testing,
        // in fact, newValue may well be NULL. Instead, we use the value
        // type specification of the newVariable and the Workspace to
        // decide on the validity of the assignment
        if ( Workspace::userWorkspace().isXOfTypeY( newVariable->getTypeSpec(), typeSpec ) )
            return true;

        return false;
    }
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


/** Remove variable intelligently */
void VariableSlot::removeVariable(void) {
    
    if ( variable != NULL ) {
        variable->removeSlot( this );
        if ( variable->numRefs() == 0 )
            delete variable;
    }

    variable = NULL;
}


/** Set a reference slot to a new variable reference */
void VariableSlot::setReference( DAGNode* newRef ) {

    if ( !isReference() )
        throw RbException("Cannot set value slot with a reference");

    setVariable( newRef );
}


/** Set reference flag of slot */
void VariableSlot::setReferenceFlag(bool refFlag) {

    /* Make sure we have an independent copy if it becomes a value slot */
    if ( refFlag == false && typeSpec.isReference() == true ) {
        if ( variable != NULL && variable->getSlot() != this ) {
            variable = variable->clone();
            variable->setSlot( this );
        }
    }
    
    if ( refFlag == true && typeSpec.isReference() == false ) {
    
        if ( variable != NULL ) {
            if ( !variable->isDAGType( ConstantNode_name ) )
                throw RbException( "Cannot set reference to nonconst temp variable" );
            variable = variable->clone();
            variable->setSlot( this );
        }
    }

    typeSpec.setReference( refFlag );
}


/**
 * Set a slot to a new value. The slot does not do
 * any type conversion; it simply checks the type
 * and throws an error if the type is wrong.
 */
void VariableSlot::setValue(RbObject* newVal) {

    // Convert to a variable
    DAGNode* newVar;
    if ( newVal == NULL ) {

        if ( variable->isDAGType( MemberObject_name ) )
            newVar  = new MemberNode( typeSpec.getType() );
        else if ( typeSpec.getDim() == 0 )
            newVar  = new ConstantNode( typeSpec.getType() );
        else
            newVar  = new ContainerNode( new ValueContainer( typeSpec ) );
    }
    else {

        // Wrap the value appropriately
        if ( newVal->isType( MemberObject_name ) )
            newVar  = new MemberNode( dynamic_cast<MemberObject*>( newVal ) );
        else if ( newVal->isType( Container_name ) )
            newVar  = new ContainerNode( dynamic_cast<Container*>( newVal ) );
        else
            newVar  = new ConstantNode( newVal );
    }

    // Rely on code for setting the variable
    setVariable( newVar );
}


/** Set a slot to a new variable value */
void VariableSlot::setVariable( DAGNode* newVariable ) {

    if ( !isValidVariable( newVariable ) ) {
        std::ostringstream msg;
        msg << "Invalid assignment to " << typeSpec << " " << getName();
        throw RbException( msg );
    }

    if ( !isReference() ) {

      removeVariable();

      if ( newVariable != NULL && !newVariable->isTemp() )
           variable = newVariable->clone();          // We are sent a reference, so we clone it
       else
           variable = newVariable;

      if ( variable != NULL )
          variable->setSlot( this );
    }
    else {

        // If we have a true reference and are sent NULL or a temp variable,
        // we delegate to the owner (we set the value of the slot we point to)
        if ( newVariable->isTemp() && variable->getSlot() != this ) {
            variable->getSlot()->setVariable( newVariable );
            return;
        }

        // Now set the reference variable
        removeVariable();
        if ( newVariable->isTemp() ) {
            variable = newVariable;
            variable->setSlot( this );
        }
        else {
            variable = newVariable;
            variable->addReferringSlot( this );
        }
    }
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const VariableSlot& x) {

    o << "<" << x.getTypeSpec() << ">";
    if ( x.getName() != "" )
        o << " " << x.getName();
    o << " =";
    if ( x.getTypeSpec().isReference() && x.getVariable() != NULL && x.getVariable()->getSlot() == &x )
        o << " [temp]";
    if ( x.getVariable() == NULL )
        o << " NULL";
    else {
        o << " ";
        x.getVariable()->printValue(o);
    }
    return o;
}

