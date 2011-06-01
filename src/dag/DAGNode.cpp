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
#include "ContainerNode.h"
#include "Frame.h"
#include "VectorInteger.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "RbString.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "VariableNode.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>


/** Constructor of empty node */
DAGNode::DAGNode(const std::string& valType) : children(), parents(), slot(), referringSlots(), valueType(valType), value(NULL) {
}


/** Constructor of filled node */
DAGNode::DAGNode(RbObject* val) : children(), parents(), slot(), referringSlots(), valueType(val->getType()), value(val) {
}


/**
 * Copy constructor should not copy children because it creates an
 * independent node. The new node is simply not used by any other objects
 * at this point. The parent nodes are left empty here because they are better
 * dealt with by the derived VariableNode classes, which have to maintain
 * dual copies of them (function arguments, distribution parameters,
 * or container elements).
 */
DAGNode::DAGNode( const DAGNode& x )
    : children(), parents(), slot(), referringSlots(), valueType( x.valueType ), value( NULL ) {

    if ( x.value != NULL )
        value = x.value->clone();
}


/** Destructor deletes value if not NULL */
DAGNode::~DAGNode( void ) {

    if ( numRefs() != 0 )
        throw RbException( "Cannot delete DAGNode with references" );

    if ( value )
        delete value;
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


/** Tell parser if element exists */
bool DAGNode::existsElement( VectorInteger& index ) {

    if ( index.size() == 0 )
        return true;
    else
        return false;
}


/** Get container of which this node is an element */
ContainerNode* DAGNode::getContainer( void ) const {

    for ( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        if ( (*i)->isDAGType( ContainerNode_name ) )
            return static_cast<ContainerNode*>( (*i) );
    }

    return NULL;
}


/** Get class vector describing type of DAG node */
const VectorString& DAGNode::getDAGClass() const {

    static VectorString rbClass = VectorString( DAGNode_name );
    return rbClass;
}


/** Get type of DAG node (first entry in class vector) */
const std::string& DAGNode::getDAGType( void ) const { 

    return getDAGClass()[0];
}


/** Get (make) element */
DAGNode* DAGNode::getElement( VectorInteger& index ) {

    if ( index.size() == 0 )
        return this;
    else
        throw RbException( getName() + index.toIndexString() + " does not exist" );
}


/** Get element owner */
DAGNode* DAGNode::getElementOwner( VectorInteger& index ) {

    if ( index.size() == 0 )
        return this;
    else
        throw RbException( getName() + index.toIndexString() + " does not exist" );
}


/** Get name of DAG node from its surrounding objects */
const std::string DAGNode::getName( void ) const {

    std::string name;

    if ( slot == NULL ) {
        for ( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
            if ( (*i)->isDAGType( ContainerNode_name ) ) {
                ContainerNode* theContainer = static_cast<ContainerNode*>( *i );
                name = theContainer->getName();
                VectorInteger index = theContainer->getIndex( *i );
                for ( size_t j = 0; j < index.size(); j++ )
                    name += "[" + RbString( index[j] + 1 ) + "]";
                break;
            }
        }
    }
    else {
        name = slot->getName();
    }

    return name;
}


/** Get type spec of DAG node */
const TypeSpec DAGNode::getTypeSpec( void ) const { 

    return TypeSpec( getValueType(), getDim() );
}


/** Does the variable contain a constant value? */
bool DAGNode::isConst( void ) const {

    if ( value == NULL || value->isConstant() )
        return true;

    return false;
}


/** Is DAG node of specified type? We need to check entire class vector in case we are derived from type. */
bool DAGNode::isDAGType( const std::string& type ) const {

    const VectorString& classVec = getDAGClass();

    for ( size_t i = 0; i < classVec.size(); i++ ) {
        if ( type == classVec[i] )
            return true;
    }

    return false;
}


/** Is it possible for someone to mutate this variable? */
bool DAGNode::isImmutable( void ) const {

    if ( slot != NULL || referringSlots.size() != 0 )
        return false;

    for ( std::set<DAGNode*>::const_iterator i = parents.begin(); i != parents.end(); i++ ) {
        if ( !(*i)-isImmutable() )
            return false;
    }

    ContainerNode* container = getContainer();
    if ( container == NULL )
        return true;

    if ( container->getSlot() == NULL && container->getReferringSlots().size() == 0 )
        return true;

    return false;
}


/** Is it possible to mutate node to newNode? */
bool DAGNode::isMutableTo( DAGNode* newNode ) const {

    for ( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        if ( !(*i)->isParentMutableTo( this, newNode ) )
            return false;
    }

    if ( slot != NULL && !slot->isValidVariable( newNode ) )
        return false;

    for ( std::set<VariableSlot*>::const_iterator i = referringSlots.begin(); i != referringSlots.end(); i++ ) {
        if ( ! (*i)->isValidVariable( newNode ) )
            return false;
    }

    return true;
}


/** Check if node is a parent of node x in the DAG: needed to check for cycles in the DAG */
bool DAGNode::isParentInDAG( const DAGNode* x, std::list<DAGNode*>& done ) const {

    for( std::set<DAGNode*>::const_iterator i = parents.begin(); i != parents.end(); i++ ) {

        if ( std::find( done.begin(), done.end(), (*i) ) == done.end() ) {
            if ( (*i)->isParentInDAG( x, done ) )
                return true;
        }
    }

    return false;
}


/** Is the node of language type typeSpec? */
bool DAGNode::isTypeSpec( const TypeSpec& typeSp ) const {

    return Workspace::userWorkspace().isXOfTypeY( getTypeSpec(), typeSp );
}


/** Mutate to newNode */
void DAGNode::mutateTo( DAGNode* newNode ) {
    
    // Throw an error if not possible
    if ( !isMutableTo( newNode ) )
        throw RbException( "Invalid attempt to mutate variable" );

    assert( newNode->getSlot() != NULL );

    // We need a temp vector because we will lose the children during the process
    std::set<VariableNode*> oldChildren = children;
    for ( std::set<VariableNode*>::iterator i = oldChildren.begin(); i != oldChildren.end(); i++ )
        (*i)->swapParentNode( this, newNode );

    // We need a temp vector because we will lose our referring slots during the process
    std::set<VariableSlot*> oldSlots = referringSlots;
    for ( std::set<VariableSlot*>::iterator i = oldSlots.begin(); i != oldSlots.end(); i++ )
        (*i)->setVariable( newNode );

    // Now it is up to the slot to delete us
}


/** Get number of references to the node from variable slots and other DAG nodes */
int DAGNode::numRefs( void ) const {

    if ( slot == NULL )
        return numChildren() + referringSlots.size();
    else
        return numChildren() + referringSlots.size() + 1;
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

    for ( std::set<DAGNode*>::const_iterator i = parents.begin(); i != parents.end(); i++) {
        if ( i != parents.begin() )
            o << ", ";
        if ( getName() == "" )
            o << "<" << (*i)->briefInfo() << ">";
        else
            o << (*i)->getName();
    }
    o << " ]";
}


/** Remove a slot referring to the DAG node */
void DAGNode::removeSlot( const VariableSlot* s ) {

    if ( slot == s ) {
        if ( referringSlots.size() == 0 )
            slot = NULL;
        else {
            slot = *referringSlots.begin();
            referringSlots.erase( referringSlots.begin() );
        }
    }
    else {
        std::set<VariableSlot*>::iterator i = std::find( referringSlots.begin(), referringSlots.end(), s );
        if ( i == referringSlots.end() )
            throw RbException( "Variable " + getName() + " not aware of the reference from slot " + s->getName() );
        referringSlots.erase( i );
    }
}


/** Set element: throw an appropriate exception */
void DAGNode::setElement( const VectorNatural& index, DAGNode* var, bool convert ) {

    throw RbException( "Variable " + getName() + " does not have elemments" );
}

