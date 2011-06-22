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
#include "ContainerNode.h"
#include "ConverterNode.h"
#include "DAGNode.h"
#include "ContainerNode.h"
#include "Frame.h"
#include "LookupNode.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbOptions.h"      // For PRINTF
#include "TypeSpec.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VariableSlot.h"
#include "VectorNatural.h"
#include "Workspace.h"

#include <cassert>


/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const TypeSpec& typeSp, DAGNode* var)
    : typeSpec(typeSp), variable(NULL), frame(NULL) {

    setVariable( var );
}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const TypeSpec& typeSp)
    : typeSpec(typeSp), variable(NULL), frame(NULL) {

    setVariable( nullVariable( typeSpec ), false );
}


/** Copy constructor. We need to copy intelligently based on whether the value is a reference, a temp reference or a regular value.
    We set frame to NULL here and let caller worry about setting it to something appropriate. */
VariableSlot::VariableSlot(const VariableSlot& x) : typeSpec(x.getTypeSpec()), variable(NULL), frame(NULL) {

    if ( !typeSpec.isReference() ) {
        if ( x.variable != NULL ) {
            variable = x.variable->clone();
            variable->setSlot( this );
        }
    }
    else /* if ( typeSpec.isReference() ) */ {
        if ( x.variable->getSlot() == &x ) {
             variable = x.variable->clone();
             variable->setSlot( this );
        }
        else {
            variable = x.variable;
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
        
        // Set frame to NULL
        frame = NULL;

        // Copy the new variable
        if ( !typeSpec.isReference() ) {
            if ( x.variable != NULL ) {
                variable = x.variable->clone();
                variable->setSlot( this );
            }
        }
        else /* if (typeSpec.isReference()) */ {
            if ( x.variable->getSlot() == &x ) {
                variable = x.variable->clone();
                variable->setSlot( this );
            }
            else {
                variable = x.variable;
                variable->addReferringSlot(this);
             }
        }
    }

    return (*this);
}


/** Clone slot and variable */
VariableSlot* VariableSlot::clone( void ) const {

    return new VariableSlot( *this );
}


/** Clone slot without variable */
VariableSlot* VariableSlot::cloneEmpty( void ) const {

    return new VariableSlot( typeSpec );
}


/** Convert a value, if possible, otherwise rely on variable mutation */
RbObject* VariableSlot::convertValue( RbObject* newValue ) const {

    const TypeSpec& theTypeSpec = ( variable == NULL ? typeSpec : variable->getTypeSpec() );

    if ( newValue->isTypeSpec( theTypeSpec ) )
        return newValue;

    if ( newValue->isConvertibleTo( theTypeSpec, true ) ) {
    
        PRINTF( "Using value conversion" );
        RbObject* temp = newValue->convertTo( theTypeSpec );
        delete newValue;
        return temp;
    }

    PRINTF( "Relying on variable mutation\n" );
    return newValue;
}




/** Convert variable */
DAGNode* VariableSlot::convertVariable( DAGNode* newVariable ) const {

    const TypeSpec& theTypeSpec = ( variable == NULL ? typeSpec : variable->getTypeSpec() );

    if ( newVariable->isTypeSpec( theTypeSpec ) )
        return newVariable;

    /* If the variable is immutable, we convert it once; otherwise, we insert a converter node */
    if ( newVariable->isImmutable() ) {

        if ( newVariable->getValue()->isConvertibleTo( theTypeSpec, true ) ) {
        
            DAGNode* temp = wrapValue( newVariable->getValue()->convertTo( theTypeSpec ) );
            delete newVariable;
            return temp;
        }
    }
    else if ( Workspace::userWorkspace().isXConvertibleToY( newVariable->getTypeSpec(), theTypeSpec ) ) {

        PRINTF( "Using variable conversion" );
        return new ConverterNode( newVariable, theTypeSpec.getType(), theTypeSpec.getDim() );
    }

    PRINTF( "Relying on variable mutation\n" );

    return newVariable;
}


/** Get name of the slot from frame */
const std::string& VariableSlot::getName( void ) const {

    if ( frame == NULL )
        return EmptyString;

    return frame->getSlotName( this );
}


/**
 * Get variable for parser during evaluation of the seman-
 * tic value of an expression. Normally, the variable slot
 * should not give out a variable for others to manage
 * unless it is a reference slot.
 */
DAGNode* VariableSlot::getParserVariable( void ) {

        return variable;
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
        if ( variable->getReference()->getSlot() == this )
            return variable->clone();
        else
            return variable->getReference();
    }
}


/** Get a reference to the slot variable */
DAGNode* VariableSlot::getReference( void ) const {

    if ( !typeSpec.isReference() )
        throw RbException( "Cannot get reference to variable in value slot" );
    
    // We do not want to compute the reference here by calling variable->getReference()
    // This should be done only when we set the reference slot
    return variable;
}


/** Get the value of the variable */
const RbObject* VariableSlot::getValue( void ) const {

    return variable->getValue();
}


/**
 *  Is variable slot guaranteed to be permanent in otherFrame? This is
 *  true only if it sits in a frame that is the same or ancestral to
 *  otherFrame. By returning true directly for workspaces, we can
 *  rely on the function even for temporary frames before they have
 *  been put inside an environment, since everything sitting in
 *  any of these workspaces is safe everywhere.
 */
bool VariableSlot::isPermanent( Frame* otherFrame ) const {

    if ( frame == NULL )
        return false;

    if ( frame == &Workspace::userWorkspace() || frame == &Workspace::globalWorkspace() )
        return true;

    return frame->isSameOrParentOf( otherFrame );
}


/** Is variable valid for the slot? Additional type checking here */
bool VariableSlot::isValidVariable( DAGNode* newVariable ) const {

    return true;    // No additional requirements here, but see MemberSlot
}


/** Get appropriate NULL variable for slot */
DAGNode* VariableSlot::nullVariable( const TypeSpec& typeSp ) {

    if ( Workspace::userWorkspace().isXOfTypeY( typeSp.getType(), MemberObject_name ) )
        return new MemberNode( typeSp.getType() );
    else if ( typeSpec.getDim() > 0 )
        return new ContainerNode( new ValueContainer( typeSp ) );
    else
        return new ConstantNode( typeSp.getType() );
}


/* Print value of the slot variable */
void VariableSlot::printValue(std::ostream& o) const {

    if (variable == NULL)
        o << "NULL";
    else
        variable->printValue( o );
}


/** Remove variable intelligently */
void VariableSlot::removeVariable( void ) {
    
    if ( variable != NULL ) {
        variable->removeSlot( this );
        if ( variable->numRefs() == 0 )
            delete variable;
    }

    variable = NULL;
}


/** Replace variable without mutating old variable or doing any type conversion; used in setting argument slots */
void VariableSlot::replaceArgumentVariable( DAGNode* newVariable ) {

    if ( newVariable == NULL || variable == NULL || !newVariable->isTypeSpec( variable->getTypeSpec() ) )
        throw RbException( "Invalid attempt to replace a slot variable" );

    removeVariable();
    variable = newVariable;

    if ( variable->getSlot() == NULL )
        variable->setSlot( this );
    else
        variable->addReferringSlot( this );
}


/** Replace variable without mutating old variable or doing any type conversion; used in cloning DAGs */
void VariableSlot::replaceDAGVariable( DAGNode* newVariable ) {

    if ( newVariable == NULL || variable == NULL || !newVariable->isTypeSpec( variable->getTypeSpec() ) )
        throw RbException( "Invalid attempt to replace a slot variable" );

    // Leave slot to NULL if old variable belongs to slot outside DAG
    if ( variable->getSlot() == this )
        newVariable->setSlot( this );
    else
        newVariable->addReferringSlot( this );
    
    removeVariable();
    variable = newVariable;
}


/** Set value element */
void VariableSlot::setElement( VectorInteger& index, RbObject* newVal ) {

    setElement( index, wrapValue( newVal ), true );
}


/** Set variable element */
void VariableSlot::setElement( VectorInteger& index, DAGNode* newVar, bool convert ) {

    // Check for silly call
    if ( index.size() == 0 )
        setVariable( newVar, convert );
    
    // Check against attempt to set element to NULL
    if ( newVar == NULL )
        throw RbException( "Invalid attempt to set variable element to NULL" );

    // Delegate call
    if ( variable->isDAGType( ContainerNode_name ) ) {
    
        // Catch superfluous indices
        if ( index.size() > size_t( variable->getDim() ) ) {
        
            delete newVar;
            throw RbException( "Too many indices in setting " + variable->getName() + index.toIndexString() );
        }
    
        // Check number of indices
        size_t emptyDim = variable->getDim();
        for ( size_t i=0; i<index.size(); i++ ) {
            if ( index[i] >= 0 )
                emptyDim--;
        }

        // Get pointer to container
        ContainerNode* containerVar = static_cast<ContainerNode*>( variable );

        // Check if valid element
        if ( containerVar->isValidElement( emptyDim, newVar, convert ) ) {
        
            containerVar->setElement( index, newVar, convert );
            return;
        }

        // Attempt container mutation otherwise
        if ( containerVar->isMutableTo( newVar->getValueType(), containerVar->getDim() - emptyDim ) ) {

            ContainerNode* newContainer = containerVar->mutateTo( newVar->getValueType(), containerVar->getDim() - emptyDim );
            newContainer->setElement( index, newVar, false );
            setVariable( newContainer );
            return;
        }
        else {

            delete newVar;
            throw RbException( "Type mismatch when setting " + variable->getName() + index.toIndexString() );
        }
    }
    else if ( variable->isDAGType( MemberNode_name ) ) {

        if ( index.size() != 1 ) {
        
            delete newVar;
            throw RbException( "Too many indices in setting " + variable->getName() + index.toIndexString() );
        }

        // No variable mutation so simply delegate this to the member node
        if ( index[0] < 0 ) {
        
            delete newVar;
            throw RbException( variable->getName() + index.toIndexString() + " is not a subcontainer" );
        }

        size_t indx = size_t( index[0] );
        static_cast<MemberNode*>( variable )->setElement( indx, newVar, convert );
    }
}


/** Set reference flag of slot */
void VariableSlot::setReferenceFlag(bool refFlag) {

    /* Make sure we have an independent copy if it becomes a value slot */
    if ( refFlag == false && isReference() == true ) {
        if ( variable != NULL && variable->getSlot() != this ) {
            variable->removeSlot( this );
            variable = variable->clone();
            variable->setSlot( this );
        }
    }
    
    /* Get the reference, if it is not the same as the variable */
    if ( refFlag == true && isReference() == false ) {
    
        if ( variable != NULL ) {
            
            DAGNode* temp = variable->getReference();
            if ( temp != variable ) {
                temp->addReferringSlot( this );
                variable->removeSlot( this );
                if ( variable->numRefs() == 0 )
                    delete variable;
                variable = temp;
            }
        }
    }

    typeSpec.setReference( refFlag );
}


/** Set a slot variable to a new value */
void VariableSlot::setValue( RbObject* newValue ) {

    // If we assign a value to a true reference slot, delegate to managing slot
    if ( isReference() && variable->getReference()->getSlot() != this ) {
        variable->getReference()->getSlot()->setValue( newValue );
        return;
    }

    // Make sure we have a constant value
    if ( !newValue->isConstant() ) {

        PRINTF( "Demoting variable value to constant value" );

        if ( newValue->isType( MemberObject_name ) ) {
            RbObject* temp = static_cast<MemberObject*>( newValue )->getConstValue();
            delete newValue;
            newValue = temp;
        }
        else if ( newValue->isType( VariableContainer_name ) ) {
            RbObject* temp = static_cast<VariableContainer*>( newValue )->getConstValue();
            delete newValue;
            newValue = temp;
        }
    }

    // Make type conversion
    newValue = convertValue( newValue );

    // Wrap into a variable
    DAGNode* newVariable;
    if ( newValue == NULL )
        newVariable = nullVariable( typeSpec );
    else
        newVariable = wrapValue( newValue );

    // Make additional valitidy check
    if ( !isValidVariable( newVariable ) ) {

        delete newVariable;
        throw RbException( "Invalid assignment: new value does not fit slot requirements" );
    }

    // Fill slot, possibly replacing variable
    // We own the new variable, regardless of whether
    // we are a reference slot or a value slot
    newVariable->setSlot( this );
    if ( variable!= NULL ) {
        variable->mutateTo( newVariable );  // This will throw error if variable mutation is not possible
        removeVariable();
    }
    variable = newVariable;
}


/** Set variable */
void VariableSlot::setVariable( DAGNode* newVariable, bool convert ) {

    // TODO: Should we do redirection here if a true reference slot?
    // Or does variable mutation deal with this case correctly?

    // Check against attempt to set slot to NULL
    if ( newVariable == NULL )
        throw RbException( "Invalid attempt to set variable slot to NULL" );

    // Variable conversion
    if ( convert == true )
        newVariable = convertVariable( newVariable );

    // Clone the variable if it lives somewhere else and we are a value slot
    if ( !isReference() && newVariable->getSlot() != NULL )
        newVariable = newVariable->clone();

    // Clone the variable if it is not a safe reference
    if ( isReference() && newVariable->getSlot() != NULL && !newVariable->isPermanent( frame ) )
        newVariable = newVariable->clone();

    // Make additional validity check
    if ( !isValidVariable( newVariable ) ) {

        delete newVariable;
        throw RbException( "Invalid assignment: new value does not fit slot requirements" );
    }

    // Replace the variable
    if ( isReference() && newVariable->getSlot() != NULL )
        newVariable->addReferringSlot( this );
    else
        newVariable->setSlot( this );
    if ( variable!= NULL ) {
        variable->mutateTo( newVariable );
        removeVariable();
    }

    variable = newVariable;
}


/** Wrap value appropriately to get a variable */
DAGNode* VariableSlot::wrapValue( RbObject* value ) const {

    if ( value->isType( MemberObject_name ) )
        return new MemberNode( static_cast<MemberObject*>( value ) );
    else if ( value->isType( Container_name ) )
        return new ContainerNode( static_cast<Container*>( value ) );
    else
        return new ConstantNode( value );
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
        o << " " << x.getVariable()->briefInfo();
    }
    return o;
}

