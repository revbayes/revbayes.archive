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
#include "DAGNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "RbPtr.h"
#include "TypeSpec.h"
#include "Variable.h"
#include "VariableNode.h"
#include "VariableSlot.h"
#include "Workspace.h"

#include <cassert>


/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const std::string &lbl, const TypeSpec& typeSp, const RbPtr<Variable>& var) : RbInternal(), label(lbl) {
    
    variable = var;
    
}

/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const std::string &lbl, const RbPtr<Variable>& var) : RbInternal() , label(lbl) {
    
    variable = var;
    
}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const std::string &lbl, const TypeSpec& typeSp) : RbInternal(), label(lbl) {
    
    variable = NULL;
    
}


/** Clone slot and variable */
VariableSlot* VariableSlot::clone( void ) const {
    
    return new VariableSlot( *this );
}


/** Get class name of object */
const std::string& VariableSlot::getClassName(void) { 
    
    static std::string rbClassName = "Variable slot";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& VariableSlot::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& VariableSlot::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get a const pointer to the dag node */
const DAGNode* VariableSlot::getDagNode( void ) const {
    
    if (variable == NULL) 
        return NULL;
    else
        return variable->getDagNode();
}


/** Get a reference to the slot variable */
DAGNode* VariableSlot::getDagNode( void ) {
    
    if (variable == NULL) 
        return NULL;
    else
        return variable->getDagNode();
}


/** Get the value of the variable */
RlValue<const RbLanguageObject> VariableSlot::getValue( void ) const {
    
    const RlValue<RbLanguageObject>& tmpVal = variable->getDagNode()->getValue();
    
    std::vector<RbPtr<const RbLanguageObject> > constVals;
    for (std::vector<RbPtr<RbLanguageObject> >::const_iterator i = tmpVal.value.begin(); i != tmpVal.value.end(); ++i) {
        constVals.push_back( RbPtr<const RbLanguageObject>( *i ) );
    }
    
    RlValue<const RbLanguageObject> retVal = RlValue<const RbLanguageObject>( constVals, tmpVal.lengths );
    return retVal;
}


/** Get the value of the variable */
const RlValue<RbLanguageObject>& VariableSlot::getValue( void ) {
    
    const RlValue<RbLanguageObject>& retVal = variable->getDagNode()->getValue();
    
    return retVal;
}


const Variable& VariableSlot::getVariable(void) const {
    return *variable;
}


Variable& VariableSlot::getVariable(void) {
    return *variable;
}


const RbPtr<Variable>& VariableSlot::getVariablePtr(void) const {
    return variable;
}


/** Is variable valid for the slot? Additional type checking here */
bool VariableSlot::isValidVariable( const DAGNode& newVariable ) const {
    
    return true;    // No additional requirements here, but see MemberSlot
}



/* Print value of the slot variable */
void VariableSlot::printValue(std::ostream& o) const {
    
    if (variable == NULL)
        o << "NULL";
    else
        variable->printValue(o);
}


/** Set variable */
void VariableSlot::setVariable(const RbPtr<Variable>& var) {
    
    // set the new variable
    variable = var;
    
    // if our name is not empty we set the name of the DAG node
    if ( label != "" ) {
        var->getDagNode()->setName( label );
    }
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const VariableSlot& x) {
    
    o << "<" << x.getTypeSpec() << ">";
    if ( x.getLabel() != "" )
        o << " " << x.getLabel();
    o << " =";
    if ( x.getDagNode() == NULL )
        o << " NULL";
    else {
        o << " ";
        x.getDagNode()->printValue(o);
    }
    return o;
}

