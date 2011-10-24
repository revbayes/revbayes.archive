/**
 * @file
 * This file contains the implementation of DAGNode, which is the base
 * class for nodes in the model DAG.
 *
 * @brief Implementation of DAGNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2010-01-12 21:52:44 +0100 (Tis, 12 Jan 2010) $
 * @author The REvBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @interface Distribution
 * @extends RbObject
 *
 * $Id: DAGNode.cpp 222 2010-01-12 20:52:44Z Hoehna $
 */

#include "DAGNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbObject.h"
#include "RbOptions.h"
#include "RbString.h"
#include "Variable.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>


/** Constructor of empty node */
DAGNode::DAGNode(const std::string& valType) : children(), parents(), valueTypeSpec(valType), value(NULL) {

    // initialize the variable
    variable = NULL;
}


/** Constructor of filled node */
DAGNode::DAGNode(RbPtr<RbLanguageObject> val) : children(), parents(), valueTypeSpec(val->getType()), value(val) {
    
    // initialize the variable
    variable = NULL;
    
}


/**
 * Copy constructor should not copy children because it creates an
 * independent node. The new node is simply not used by any other objects
 * at this point. The parent nodes are left empty here because they are better
 * dealt with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode( const DAGNode& x ) : children(), parents(), valueTypeSpec(x.valueTypeSpec), value( NULL ) {

    if ( x.value.get() != NULL ) {
        value = RbPtr<RbLanguageObject>( x.value->clone() );
    }
    
//    if (x.variable != NULL) {
//        variable = x.variable->clone();
//    }
    variable = NULL;
    
    // copy the name so that we still be able to identify the variable in a cloned DAG
    name = x.name;
    
}


/** Destructor deletes value if not NULL */
DAGNode::~DAGNode( void ) {

}


/** add a child node */
void DAGNode::addChildNode(VariableNode *c) {
    PRINTF("Adding child with name \"%s\" to parent with name \"%s\".\n",c->getName().c_str(),getName().c_str());
    
    children.insert(c);
}


/** Return brief info about variable */
std::string DAGNode::briefInfo( void ) const {

    std::ostringstream o;

    if ( value == NULL )
        o << "NULL";
    else
        value->printValue( o );

    if ( o.str().size() > 10 )
        return value->briefInfo();
    else
        return o.str();
}

/** Get class vector describing type of DAG node */
const VectorString& DAGNode::getClass() const {

    static VectorString rbClass = VectorString( DAGNode_name ) + RbInternal::getClass();
    return rbClass;
}

RbPtr<RbObject> DAGNode::getElement(size_t index) {
    
    // test whether the value supports indexing, i.e. is a container
    if (value->supportsIndex()) {
        
        return value->getElement(index);
    
    } else {
        
        std::ostringstream  msg;
        msg << "Illegal access of element at index [" << index << "] in object with tpye \"" << value->getType() << "\"";
        throw RbException( msg );
    }
    
}

/** Get name of DAG node from its surrounding objects */
const std::string& DAGNode::getName( void ) const {

    return name;
}



/** Is this a constant node */
bool DAGNode::isConst( void ) const {

    return false;
}


/** Check if node is a parent of node x in the DAG: needed to check for cycles in the DAG */
bool DAGNode::isParentInDAG( const RbPtr<DAGNode> x, std::list<DAGNode*>& done ) const {

    for( std::set<RbPtr<DAGNode> >::const_iterator i = parents.begin(); i != parents.end(); i++ ) {

        if ( std::find( done.begin(), done.end(), (*i) ) == done.end() ) {
            if ( (*i)->isParentInDAG( x, done ) )
                return true;
        }
    }

    return false;
}


/** Is the node of language type typeSpec? */
bool DAGNode::isValueOfTypeSpec( const TypeSpec& typeSp ) const {

    return value->isTypeSpec( typeSp );
}

/** Print children */
void DAGNode::printChildren( std::ostream& o ) const {

    o << "[ ";

    for ( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++) {
        if ( i != children.begin() )
            o << ", ";
        if ( getName() == "" )
            o << "<" << (*i)->briefInfo() << ">";
        else
            o << (*i)->getName();
    }
    o << " ]";
}


/** Print parents */
void DAGNode::printParents( std::ostream& o ) const {

    o << "[ ";

    for ( std::set<RbPtr<DAGNode> >::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( i != parents.begin() )
            o << ", ";
        if ( getName() == "" )
            o << "<" << (*i)->briefInfo() << ">";
        else
            o << (*i)->getName();
    }
    o << " ]";
}


void DAGNode::setVariable(Variable *var) {
    // only do something if the old var is different to the new var
    if (var != variable) {
        // the DAG node does not own the Variable so we do not release it
    
        // set the new variable
        variable = var;
        
        name = var->getName();
        
        // we do not retain the variable for now because if we would, then the DAG node holds a pointer to the variable
    }
}

/** Remove a child from this DAG node. Free the pointer to the child. */
void DAGNode::removeChildNode(VariableNode *c) {
    
    // test if we even have this node as a child
    if (children.find(c) != children.end()) {
        // we do not own our children! See addChildNode for explanation
//        c->release();
//        if (c->isUnreferenced()) {
//            delete c;
//        }
        
        // remove the child from our list
        children.erase(c);
    }
}

