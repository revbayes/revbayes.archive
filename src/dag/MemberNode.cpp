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


#include "Boolean.h"
#include "MemberFrame.h"
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
MemberNode::MemberNode( const std::string& valType )
    : DeterministicNode( valType ), memberObject( NULL ) {
}


/** Basic constructor of member node */
MemberNode::MemberNode( MemberObject* val )
    : DeterministicNode( val->getType() ), memberObject( val ) {

    /* Check for cycles */
    std::list<DAGNode*> done;
    const MemberFrame& members = val->getMembers();
    for ( size_t i = 0; i < members.size(); i++ ) {
        if ( members[i].getVariable()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }

    /* Set parents and add this node as a child */
    for ( size_t i = 0; i < members.size(); i++ ) {
        DAGNode* theNode = const_cast<DAGNode*>( members[i].getVariable() );
        parents.insert( theNode );
        theNode->addChildNode( this );
    }

    /* Set value and stored value */
    touched     = false;
    changed     = false;
    value       = memberObject->getConstValue();
    storedValue = NULL;
}


/** Destructor */
MemberNode::~MemberNode( void ) {

    if ( numRefs() != 0 )
        throw RbException( "Cannot delete MemberNode with references" ); 

    /* Remove parents first so that DeterministicNode destructor does not get in the way */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode( this );
    parents.clear();

    delete memberObject;    // This will delete any DAG nodes that need to be deleted
}


/** Clone this object */
MemberNode* MemberNode::clone( void ) const {

	return new MemberNode( *this );
}


/** Clone the entire graph: clone children, swap parent */
MemberNode* MemberNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<MemberNode*>( newNodes[ this ] );

    /* Get pristine copy */
    MemberNode* copy = new MemberNode( valueType );
    newNodes[ this ] = copy;

    /* Clone member object and value */
    copy->memberObject = memberObject->clone();
    copy->value = value->clone();
    if ( storedValue != NULL )
        copy->storedValue = storedValue->clone();
    else
        copy->storedValue = NULL;
    copy->touched = touched;
    copy->changed = changed;

    /* Set parents and add this node as a child */
    const MemberFrame& members     = memberObject->getMembers();
    MemberFrame&       copyMembers = const_cast<MemberFrame&>( copy->memberObject->getMembers() );

    for ( size_t i = 0; i < members.size(); i++ ) {

        DAGNode* theMemberClone = members[i].getVariable()->cloneDAG( newNodes );
        copyMembers[i].resetVariable( theMemberClone );

        copy->parents.insert( theMemberClone );
        theMemberClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
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

    static VectorString rbClass = VectorString( MemberNode_name ) + DeterministicNode::getDAGClass();
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
const TypeSpec& MemberNode::getMemberTypeSpec( const RbString& name ) const {

    return memberObject->getMemberTypeSpec( name );
}


/** Print struct for user */
void MemberNode::printStruct(std::ostream& o) const {

    o << "_DAGClass    = " << getDAGClass() << std::endl;
    o << "_valueType   = " << valueType << std::endl;
    o << "_dim         = " << getDim() << std::endl;
    o << "_touched     = " << ( touched ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_changed     = " << ( changed ? Boolean( true ) : Boolean( false ) ) << std::endl;
    o << "_value       = " << value->briefInfo() << std::endl;
    if ( touched && changed )
        o << "_storedValue = " << storedValue->briefInfo() << std::endl;

    o << "_parent      = ";
    printParents(o);
    o << std::endl;

    o << "_children    = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Complete info about object */
std::string MemberNode::richInfo(void) const {

    std::ostringstream o;

    o << "MemberNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << ( touched ? "true" : "false" ) << std::endl;
    o << "changed     = " << ( changed ? "true" : "false" ) << std::endl;
    o << "valueType   = " << valueType << std::endl;

    o << "value = ";
    value->printValue( o );
    o << std::endl;

    if ( storedValue ) {
        o << "storedValue = ";
        storedValue->printValue( o );
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


/** Update value and stored value after node and its surroundings have been touched by a move */
void MemberNode::update(void) {

    if (touched && !changed) {

        assert( storedValue == NULL );

        storedValue = value;
        value       = memberObject->getConstValue();
        changed     = true;
    }
}

