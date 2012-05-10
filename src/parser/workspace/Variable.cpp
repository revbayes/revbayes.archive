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

#include <cassert>
#include <string>
#include <sstream>


/** Constructor of filled variable. */
Variable::Variable(const TypeSpec& ts) : RbInternal(), valueTypeSpec( ts ), node( NULL ) {
    
}

/** Constructor of filled variable. */
Variable::Variable(const RbPtr<DAGNode> &n): valueTypeSpec( RbLanguageObject::getClassTypeSpec() ) {
    
    // initialize the variable
    node = NULL;
    
    setDagNode( n );
    
}


/* Clone variable and variable */
Variable* Variable::clone( void ) const {
    
    return new Variable( *this );
}


/* Get class name of object */
const std::string& Variable::getClassName(void) { 
    
    static std::string rbClassName = "Variable";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Variable::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbInternal::getClassTypeSpec() ) );
    
	return rbClass; 
}

/* Get type spec */
const TypeSpec& Variable::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


RbPtr<const DAGNode> Variable::getDagNode(void) const {
    return RbPtr<const DAGNode>( node );
}


const RbPtr<DAGNode>& Variable::getDagNode(void) {
    return node;
}


/* Get the value of the variable */
const RbLanguageObject& Variable::getValue(void) const {

    if (node == NULL) {
        return RbNullObject::getInstance();
    }
    
    RbLanguageObject& retVal = node->getValue();
    
    return retVal;
}

/** Get the value of the variable */
RbLanguageObject& Variable::getValue(void) {
    
    RbLanguageObject& retVal = node->getValue();
    
    return retVal;
}


/** Get the required type specs for values stored inside this variable */
const TypeSpec& Variable::getValueTypeSpec(void) const {
    return valueTypeSpec;
}



/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const {
    
    if (node == NULL)
        o << "NULL";
    else
        node->printValue( o );
}


/** Set variable */
void Variable::setDagNode( const RbPtr<DAGNode> &newVariable ) {
    
    // change the old variable with the new variable in the parent and children
    replaceDagNode( newVariable );
    
    // set the variable for the new variable
//    node->setVariable( this );
}


/** Replace DAG node, only keep the children */
void Variable::replaceDagNode( const RbPtr<DAGNode> &newVariable) {
    
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


/** We set here the required value type spec. */
void Variable::setValueTypeSpec(const TypeSpec &ts) {
    valueTypeSpec = ts;
}


