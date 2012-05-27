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
    
    variable.push_back( var );
    
}

/** Constructor of filled slot with type specification. */
VariableSlot::VariableSlot(const std::string &lbl, const RbPtr<Variable>& var) : RbInternal() , label(lbl) {
    
    variable.push_back( var );
    
}


/** Constructor of empty slot based on type specification */
VariableSlot::VariableSlot(const std::string &lbl, const TypeSpec& typeSp) : RbInternal(), label(lbl) {
    
    variable.push_back( NULL );
    
}


/** Clone slot and variable */
VariableSlot* VariableSlot::clone( void ) const {
    
    return new VariableSlot( *this );
}


void VariableSlot::createVariable(const std::vector<int> &indices) {
    // test whether we need to update the dimensions
    if ( indices.size() > lengths.size() && lengths.size() > 0 ) {
        throw RbException("Cannot resize dimension of slot variable that already exists!");
    }
    else if ( indices.size() > lengths.size() ) {
        // this was a slot for a single variable before
        
        // fill with empty variables
        size_t nElements = 1;
        for (size_t i = 0; i < indices.size(); ++i) {
            nElements *= (indices[i]+1);
            lengths.push_back( indices[i]+1 );
        }
        for (size_t i = 0; i < nElements; ++i) {
            variable.push_back( NULL );
        }

    }
    else {
        for (size_t i = 0; i < indices.size(); ++i) {
            if ( indices[i] > lengths[i] ) {
                throw RbException();
            }
        }
    }
    
    // create a new lengths vector, if necessary
}


bool VariableSlot::doesVariableExist(const std::vector<int> &indices) const {
    // test whether we need to update the dimensions
    if ( indices.size() > lengths.size() ) {
        return false;
    } else {
        for (size_t i = 0; i < indices.size(); ++i) {
            if ( indices[i] > lengths[i] ) {
                return false;
            }
        }
    }
    
    return true;
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


size_t VariableSlot::getDim( void ) const {
    return lengths.size();
}

/** Get type spec */
const TypeSpec& VariableSlot::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get a const pointer to the dag node */
const DAGNode* VariableSlot::getDagNode( void ) const {
    
    if (variable[0] == NULL) 
        return NULL;
    else
        return variable[0]->getDagNode();
}


/** Get a reference to the slot variable */
DAGNode* VariableSlot::getDagNode( void ) {
    
    if (variable[0] == NULL) 
        return NULL;
    else
        return variable[0]->getDagNode();
}


/** Get the value of the variable */
RlValue<const RbLanguageObject> VariableSlot::getValue( void ) const {
    
    const RlValue<RbLanguageObject>& tmpVal = variable[0]->getDagNode()->getValue();
    
    std::vector<RbPtr<const RbLanguageObject> > constVals;
    for (std::vector<RbPtr<RbLanguageObject> >::const_iterator i = tmpVal.value.begin(); i != tmpVal.value.end(); ++i) {
        constVals.push_back( RbPtr<const RbLanguageObject>( *i ) );
    }
    
    RlValue<const RbLanguageObject> retVal = RlValue<const RbLanguageObject>( constVals, tmpVal.lengths );
    return retVal;
}


/** Get the value of the variable */
const RlValue<RbLanguageObject>& VariableSlot::getValue( void ) {
    
    const RlValue<RbLanguageObject>& retVal = variable[0]->getDagNode()->getValue();
    
    return retVal;
}


const Variable& VariableSlot::getVariable(void) const {
    return *variable[0];
}


Variable& VariableSlot::getVariable(void) {
    return *variable[0];
}


const RbPtr<const Variable>& VariableSlot::getVariable(const std::vector<int> &indices) const {
    size_t index = 0;
    size_t elements = 1;
    
    if (indices.size() != lengths.size()) {
        throw RbException("Index out of bounds! Unequal indices for variables not supported (yet).");
    }
    
    for (int i = int(lengths.size())-1; i >= 0; --i) {
        // test for boundaries
        if (indices[i] >= lengths[i]) {
            throw RbException("Index out of bounds! Cannot access variable with index ...");
        }
        index += indices[i] * elements;
        elements *= lengths[i];
    }
    
    
    return RbPtr<const Variable>( variable[index] );
}


const RbPtr<Variable>& VariableSlot::getVariable(const std::vector<int> &indices) {
    size_t index = 0;
    size_t elements = 1;
    
    if (indices.size() != lengths.size()) {
        throw RbException("Index out of bounds! Unequal indices for variables not supported (yet).");
    }
    
    for (int i = int(lengths.size())-1; i >= 0; --i) {
        // test for boundaries
        if (indices[i] >= lengths[i]) {
            throw RbException("Index out of bounds! Cannot access variable with index ...");
        }
        index += indices[i] * elements;
        elements *= lengths[i];
    }
    
    
    return variable[index];
}


const RbPtr<Variable>& VariableSlot::getVariablePtr(void) const {
    return variable[0];
}


/** Is variable valid for the slot? Additional type checking here */
bool VariableSlot::isValidVariable( const DAGNode& newVariable ) const {
    
    return true;    // No additional requirements here, but see MemberSlot
}



/* Print value of the slot variable */
void VariableSlot::printValue(std::ostream& o) const {
    
    if (variable[0] == NULL)
        o << "NULL";
    else
        variable[0]->printValue(o);
}


/** Set variable */
void VariableSlot::setVariable(const RbPtr<Variable>& var) {
    
    // set the new variable
    variable[0] = var;
    
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

