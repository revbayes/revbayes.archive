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
Variable::Variable(const std::string &n, RbPtr<DAGNode> dn) : RbInternal() {
    name = n;
    
    // initialize the variable
    setDagNode( dn );
    
    // notify the variable that this is the new variable
    dn->setVariable( this );
}

/** Constructor of filled variable. */
Variable::Variable(RbPtr<DAGNode> n) {
    // we do not have a name for the variable so we ask the dag node
    name = n->getName();
    
    // initialize the variable
    node = NULL;
    
    setDagNode( n );
    
    // notify the variable that this is the new variable
    n->setVariable( this );
}

/** Constructor of without variable. */
Variable::Variable(const std::string &n) : node(NULL) {
    name = n;
}


/** Copy constructor. We need to copy intelligently based on whether the value is a reference, a temp reference or a regular value.
 We set frame to NULL here and let caller worry about setting it to something appropriate. */
Variable::Variable(const Variable& x) : node(NULL)  {
    
    if ( x.node != NULL ) {
        node = RbPtr<DAGNode>( x.node->clone() );
    }
    
    // copy the name too
    name = x.name;
}


/** Call a help function to remove the variable intelligently */
Variable::~Variable(void) {
    
}


/** Assignment operator */
Variable& Variable::operator=(const Variable& x) {
    
    if ( &x != this ) {
        
        // Copy the new variable
        node = x.node;
        name = x.name;
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


RbPtr<const DAGNode> Variable::getDagNode(void) const {
    return RbPtr<const DAGNode>( node );
}


RbPtr<DAGNode> Variable::getDagNode(void) {
    return ( node );
}


/** Get name of the variable from frame */
const std::string& Variable::getName( void ) const {
    
    return name;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Variable::getTypeSpec(void) const {
    return typeSpec;
}


/** Get the value of the variable */
RbPtr<const RbObject> Variable::getValue(void) const {
    return node->getValue();
}

/** Get the value of the variable */
RbPtr<RbObject> Variable::getValue(void) {
    return node->getValue();
}


/* Print value of the variable variable */
void Variable::printValue(std::ostream& o) const {
    
    if (node == NULL)
        o << "NULL";
    else
        node->printValue( o );
}


/** Set variable */
void Variable::setDagNode( RbPtr<DAGNode> newVariable ) {
    
    // change the old variable with the new variable in the parent and children
    replaceDagNode( newVariable );
    
    // set the variable for the new variable
    node->setVariable( this );
}


/** Replace DAG node, only keep the children */
void Variable::replaceDagNode(RbPtr<DAGNode> newVariable) {
    
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


/** Replace the name of the variable. Make sure that also the DAG node changes its name */
void Variable::setName(std::string const &n) {
    name = n;
    
    // call the DAG node to change its name
    if (node != NULL) {
        node->setName(n);
    }
}


/** Make sure we can print to stream using << operator */
std::ostream& operator<<(std::ostream& o, const Variable& x) {
    
    o << "<" << x.getTypeSpec() << ">";
    if ( x.getName() != "" )
        o << " " << x.getName();
    o << " =";
    if ( x.getDagNode() == NULL )
        o << " NULL";
    else {
        o << " " << x.getDagNode()->briefInfo();
    }
    return o;
}

