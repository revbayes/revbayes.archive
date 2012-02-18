/**
 * @file
 * This file contains the implementation of Variable, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Implementation of Variable
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
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "TypeSpec.h"
#include "Variable.h"
#include "VariableNode.h"
#include "VectorString.h"

#include <cassert>
#include <string>
#include <sstream>


// Definition of the static type spec member
const TypeSpec Variable::typeSpec(Variable_name);

/** Constructor of filled variable. */
Variable::Variable(const TypeSpec& ts) : RbInternal(), valueTypeSpec( ts ), node( NULL ) {
    
}

/** Constructor of filled variable. */
Variable::Variable(DAGNode* n): valueTypeSpec( RbLanguageObject_name ) {
    
    // initialize the variable
    node = NULL;
    
    setDagNode( n );
    
    // notify the variable that this is the new variable
//    n->setVariable( this );
}


/** Copy constructor. */
Variable::Variable(const Variable& x) : node(NULL), valueTypeSpec( x.valueTypeSpec )  {
    
    if ( x.node != NULL ) {
        // We do not clone the DAG node because we just create a new variable
        // holding currently the same DAG node
        setDagNode( x.node );
        
        // notify the variable that this is the new variable
//        node->setVariable( this );
    }
    
}


/** Call a help function to remove the variable intelligently */
Variable::~Variable(void) {
    
}


/** Assignment operator */
Variable& Variable::operator=(const Variable& x) {
    
    if ( &x != this ) {
        
        // Copy the new variable
        node = x.node;
        valueTypeSpec = x.valueTypeSpec;
    }
    
    return (*this);
}


/** Clone variable and variable */
Variable* Variable::clone( void ) const {
    
    return new Variable( *this );
}

/** Get class vector describing type of object */
const VectorString& Variable::getClass() const {
    
    static VectorString rbClass = VectorString(Variable_name) + RbInternal::getClass();
    return rbClass;
}


const DAGNode* Variable::getDagNode(void) const {
    return node;
}


DAGNode* Variable::getDagNode(void) {
    return node;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Variable::getTypeSpec(void) const {
    return typeSpec;
}


/** Get the value of the variable */
const RbLanguageObject& Variable::getValue(void) const {

    if (node == NULL) {
        return RbNullObject::getInstance();
    }
    
    RbLanguageObject& retVal = node->getValue();
    
    // check the type and if we need conversion
    if (!retVal.isTypeSpec(valueTypeSpec)) {
        
        //  variable->getDagNode()->setValue(convRetVal);
        
        //TODO @Sebastian: set the new requirements to the variable that we need a converted type ...
        
        return *dynamic_cast<RbLanguageObject*>(retVal.convertTo(valueTypeSpec));
    }
    
    return retVal;
}

/** Get the value of the variable */
RbLanguageObject& Variable::getValue(void) {
    
    RbLanguageObject& retVal = node->getValue();
    
    // check the type and if we need conversion
    if (!retVal.isTypeSpec(valueTypeSpec)) {
        
        //  variable->getDagNode()->setValue(convRetVal);
        
        //TODO @Sebastian: set the new requirements to the variable that we need a converted type ...
        
        return *dynamic_cast<RbLanguageObject*>(retVal.convertTo(valueTypeSpec));
    }
    
    return retVal;
}


/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const {
    
    if (node == NULL)
        o << "NULL";
    else
        node->printValue( o );
}


/** Set variable */
void Variable::setDagNode( DAGNode *newVariable ) {
    
    // change the old variable with the new variable in the parent and children
    replaceDagNode( newVariable );
    
    // set the variable for the new variable
//    node->setVariable( this );
}


/** Replace DAG node, only keep the children */
void Variable::replaceDagNode( DAGNode *newVariable) {
    
    if (node != NULL) {
        // replace the old variable with the new variable for the parents of the old variable
        std::set<VariableNode*> children = node->getChildren();
        
        // check if the old node had parents and if the new node is not
        
        for ( std::set<VariableNode*>::iterator it=children.begin(); it!=children.end(); it++ ) {
            (*it)->swapParentNode( node, newVariable );        // this will replace the parent
        }

    }
    
    node = newVariable;
    
}


