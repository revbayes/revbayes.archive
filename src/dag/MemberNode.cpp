/**
 * @file
 * This file contains the partial implementation of MemberNode, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of MemberNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "Frame.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"

#include <assert.h>


/** Constructor of empty member node */
MemberNode::MemberNode(const std::string& valType)
    : DeterministicNode(valType), memberObject(NULL) {
}


/** Basic constructor of member node */
MemberNode::MemberNode(MemberObject* val)
    : DeterministicNode(val->getType()), memberObject(val) {

    /* Check for cycles */
    std::list<DAGNode*> done;
    const VariableTable& params = val->getMembers().getVariableTable();
    for ( VariableTable::const_iterator i=params.begin(); i!=params.end(); i++ ) {
        if ( (*i).second.getReference()->isParentInDAG( this, done ) )
            throw RbException ("Invalid assignment: cycles in the DAG");
    }

    /* Set parents and add this node as a child */
    for ( VariableTable::const_iterator i=params.begin(); i!=params.end(); i++ ) {
        DAGNode* theNode = const_cast<DAGNode*>( (*i).second.getReference() );
        parents.insert( theNode );
        theNode->addChildNode(this);
    }

    /* Set value and stored value */
    value       = memberObject->getConstValue();
    storedValue = NULL;
}


/** Destructor */
MemberNode::~MemberNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete MemberNode with references"); 

    /* Remove parents first so that DeterministicNode destructor does not get in the way */
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
        (*i)->removeChildNode(this);
    parents.clear();

    delete memberObject;    // This will delete any DAG nodes that need to be deleted
}


/** Clone this object */
MemberNode* MemberNode::clone(void) const {

	return new MemberNode(*this);
}


/** Clone the entire graph: clone children, swap parent */
MemberNode* MemberNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if (newNodes.find((DAGNode*)(this)) != newNodes.end())
        return (MemberNode*)(newNodes[(DAGNode*)(this)]);

    /* Get pristine copy */
    MemberNode* copy = new MemberNode(valueType);
    newNodes[(DAGNode*)(this)] = copy;

    /* Clone member object and value */
    //copy->memberObject = memberObject->cloneDAG(newNodes);
    copy->value = copy->memberObject->getConstValue();
    copy->storedValue = NULL;
    copy->touched = false;
    copy->changed = false;

    /* Set parents and add this node as a child */
    const VariableTable& params = copy->memberObject->getMembers().getVariableTable();
    for ( VariableTable::const_iterator i=params.begin(); i!=params.end(); i++ ) {
        DAGNode* theNode = const_cast<DAGNode*>( (*i).second.getReference() );
        copy->parents.insert( theNode );
        theNode->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Does element referred to by index exist? */
bool MemberNode::existsElement( VectorInteger& index ) {

    if ( index.size() == 0 )
        return true;
    
    if ( index[0] < 0 )
        return false;
    
    if ( size_t( index[0] ) >= memberObject->getSubelementsSize() )
        return false;

    // Pop one index and delegate to subscript element
    size_t i = index[0];
    index.pop_front();
    return memberObject->getSubelement( i )->existsElement( index );
}


/** Get class vector describing type of DAG node */
const VectorString& MemberNode::getDAGClass() const {

    static VectorString rbClass = VectorString(MemberNode_name) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Get element for parser */
DAGNode* MemberNode::getElement( VectorInteger& index ) {

    if ( index.size() == 0 )
        return this;
    
    if ( index[0] < 0 )
        throw RbException( getName() + index.toIndexString() + " is not a subcontainer" );
    
    if ( size_t( index[0] ) >= memberObject->getSubelementsSize() )
        throw RbException ( "Index out of bound for " + getName() );

    // Pop one index and delegate to subscript element
    size_t i = index[0];
    index.pop_front();
    return memberObject->getSubelement( i )->getElement( index );
}


/** Get element reference for parser */
DAGNode* MemberNode::getElementRef( VectorNatural& index ) {

    // Invalidate current value
    touched = true;
    changed = false;

    // Get reference to the element
    if ( index.size() == 0 )
        return this;
    
    if ( index[0] >= memberObject->getSubelementsSize() )
        throw RbException ( "Index out of bound for " + getName() );

    // Pop one index and delegate to subscript element
    // if permitted by our member object
    DAGNode* subElement = memberObject->getSubelement( index[0] );
    if ( subElement->isTemp() ) {

        // Not permitted by the member object
        delete subElement;
        return this;
    }
    else {

        // Permitted by the member object
        size_t i = index[0];
        index.pop_front();
        return subElement->getElementRef( index );
    }
}


/** Get type of a named member variable */
const TypeSpec& MemberNode::getMemberTypeSpec(const RbString& name) const {

    return memberObject->getMemberTypeSpec( name );
}


/** Is it possible to mutate node to newNode? */
bool MemberNode::isMutableTo(const DAGNode* newNode) const {

    return false;
}


/** Is it possible to change parent node oldNode to newNode? */
bool MemberNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find out if node is parent
    if (*parents.begin() != const_cast<DAGNode*>(oldNode))
        throw RbException("Node is not a parent");

    /* Find member and see if it is mutable to newNode */
    const VariableTable& members = memberObject->getMembers().getVariableTable();
    VariableTable::const_iterator it;
    for ( it=members.begin(); it!=members.end(); it++ ) {
        if ( (*it).second.getReference() == oldNode )
            break;
    }
    if ( it == members.end() )
        throw RbException("Node is not member variable");

    return (*it).second.getReference()->isMutableTo(newNode);
}


/** Mutate to newNode */
void MemberNode::mutateTo(DAGNode* newNode) {
    
    throw RbException("Not implemented yet");
}


/** Print struct for user */
void MemberNode::printStruct(std::ostream& o) const {

    if (touched)
        throw RbException("Cannot print struct while in touched state");

    o << "DAGNode:" << std::endl;
    o << "_class    = " << getDAGClass() << std::endl;
    o << "_value = " << std::endl;
    value->printValue(o);
    o << std::endl;
    o << "_parent   = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "_children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Complete info about object */
std::string MemberNode::richInfo(void) const {

    std::ostringstream o;

    o << "MemberNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;
    o << "valueType   = " << valueType << std::endl;

    o << "value = ";
    value->printValue(o);
    o << std::endl;

    if ( storedValue ) {
        o << "storedValue = ";
        storedValue->printValue(o);
        o << std::endl;
    }

    return o.str();
}


/** Set element for parser, if our member object wants this (see getElementRef) */
void MemberNode::setElement( VectorNatural& index, DAGNode* var ) {

    if ( index.size() == 0 )
        throw RbException( "Unexpected call to setElement of variable " + getName() );

    if ( index[0] >= memberObject->getSubelementsSize() )
        throw RbException( "Index out of bound for " + getName() );

    // Invalid current value and then set element
    touched = true;
    changed = false;
    memberObject->setElement( index, var );
}


/** Swap parent node */
void MemberNode::swapParentNode(DAGNode* oldNode, DAGNode* newNode) {

    if (parents.find(oldNode) == parents.end())
        throw RbException("Node is not parent");
    
    /* Set parents and add this node as a child */
    const VariableTable& members = memberObject->getMembers().getVariableTable();
    VariableTable::const_iterator it;
    for ( it=members.begin(); it!=members.end(); it++ ) {
        if ( (*it).second.getReference() == oldNode )
            break;
    }
    if ( it == members.end() )
        throw RbException("Node is not member variable");

    memberObject->setVariable( (*it).first, newNode );

    oldNode->removeChildNode(this);
    newNode->addChildNode(this);
    parents.erase(oldNode);
    parents.insert(newNode);

    touched = true;
    changed = false;
    touchAffected();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void MemberNode::update(void) {

    if (touched && !changed) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        value = memberObject->getConstValue();
        changed = true;
    }
}

