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
 * $Id:$
 */


#include "Boolean.h"
#include "List.h"
#include "MemberFrame.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "VectorIndex.h"
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
    const MemberFrame& members = memberObject->getMembers();
    for ( size_t i = 0; i < members.size(); i++ ) {
        if ( members[i].getVariable()->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    if ( memberObject->isType( List_name ) ) {
        const ArgumentFrame& elements = static_cast<List*>( val )->getElements();
        for ( size_t i = 0; i < elements.size(); i++ ) {
            if ( elements[i].getVariable()->isParentInDAG( this, done ) )
                throw RbException( "Invalid assignment: cycles in the DAG" );
        }
    }

    /* Set parents and add this node as a child */
    for ( size_t i = 0; i < members.size(); i++ ) {
        DAGNode* theNode = const_cast<DAGNode*>( members[i].getVariable() );
        parents.insert( theNode );
        theNode->addChildNode( this );
    }
    if ( memberObject->isType( List_name ) ) {
        const ArgumentFrame& elements = static_cast<List*>( val )->getElements();
        for ( size_t i = 0; i < elements.size(); i++ ) {
            DAGNode* theNode = const_cast<DAGNode*>( elements[i].getVariable() );
            parents.insert( theNode );
            theNode->addChildNode( this );
        }
    }

    /* Anchor member object frame */
    memberObject->members.setOwner( this );

    /* Set value and stored value */
    touched     = false;
    changed     = false;
    value       = memberObject->cloneWithoutConnections();
    storedValue = NULL;

    /* Anchor value member object frame */
    static_cast<MemberObject*>( value )->members.setOwner( this );
}


/** Copy constructor */
MemberNode::MemberNode( const MemberNode& x )
    : DeterministicNode( x ), memberObject( NULL ) {

    if ( x.memberObject != NULL ) {

        memberObject = x.memberObject->clone();

        /* Check for cycles */
        std::list<DAGNode*> done;
        const MemberFrame& members = memberObject->getMembers();
        for ( size_t i = 0; i < members.size(); i++ ) {
            if ( members[i].getVariable()->isParentInDAG( this, done ) )
                throw RbException( "Invalid assignment: cycles in the DAG" );
        }
        if ( memberObject->isType( List_name ) ) {
            const ArgumentFrame& elements = static_cast<List*>( memberObject )->getElements();
            for ( size_t i = 0; i < elements.size(); i++ ) {
                if ( elements[i].getVariable()->isParentInDAG( this, done ) )
                    throw RbException( "Invalid assignment: cycles in the DAG" );
            }
        }

        /* Set parents and add this node as a child */
        for ( size_t i = 0; i < members.size(); i++ ) {
            DAGNode* theNode = const_cast<DAGNode*>( members[i].getVariable() );
            parents.insert( theNode );
            theNode->addChildNode( this );
        }
        if ( memberObject->isType( List_name ) ) {
            const ArgumentFrame& elements = static_cast<List*>( memberObject )->getElements();
            for ( size_t i = 0; i < elements.size(); i++ ) {
                DAGNode* theNode = const_cast<DAGNode*>( elements[i].getVariable() );
                parents.insert( theNode );
                theNode->addChildNode( this );
            }
        }

        /* Anchor member object frame */
        memberObject->members.setOwner( this );

        /* Set value and stored value */
        touched     = false;
        changed     = false;
        value       = memberObject->cloneWithoutConnections();
        storedValue = NULL;

        /* Anchor value member object frame */
        static_cast<MemberObject*>( value )->members.setOwner( this );
    }
    else {

        /* Set value and stored value */
        touched     = false;
        changed     = false;
        value       = NULL;
        storedValue = NULL;
    }
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


/** Assignment operator */
MemberNode& MemberNode::operator=( const MemberNode& x ) {

    if ( this != &x ) {
        
        /* Check for type compatibility */
        if ( x.memberObject == NULL || !x.memberObject->isType( getValueType() ) )
            throw RbException( "Invalid assignment: type mismatch in member object" );

        /* Check for cycles */
        std::list<DAGNode*> done;
        const MemberFrame& xMembers = x.memberObject->getMembers();
        for ( size_t i = 0; i < xMembers.size(); i++ ) {
            if ( xMembers[i].getVariable()->isParentInDAG( this, done ) )
                throw RbException( "Invalid assignment: cycles in the DAG" );
        }
        if ( x.memberObject->isType( List_name ) ) {
            const ArgumentFrame& elements = static_cast<List*>( x.memberObject )->getElements();
            for ( size_t i = 0; i < elements.size(); i++ ) {
                if ( elements[i].getVariable()->isParentInDAG( this, done ) )
                    throw RbException( "Invalid assignment: cycles in the DAG" );
            }
        }

        /* Now we can go ahead with the assignment. Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
            (*i)->removeChildNode( this );
        parents.clear();

        delete memberObject;    // This will delete any DAG nodes that need to be deleted
        delete value;
        memberObject = NULL;
        value        = NULL;

        /* Clone member object */
        memberObject = x.memberObject->clone();

        /* Set parents and add this node as a child */
        const MemberFrame& members = memberObject->getMembers();
        for ( size_t i = 0; i < members.size(); i++ ) {
            DAGNode* theNode = const_cast<DAGNode*>( members[i].getVariable() );
            parents.insert( theNode );
            theNode->addChildNode( this );
        }
        if ( memberObject->isType( List_name ) ) {
            const ArgumentFrame& elements = static_cast<List*>( memberObject )->getElements();
            for ( size_t i = 0; i < elements.size(); i++ ) {
                DAGNode* theNode = const_cast<DAGNode*>( elements[i].getVariable() );
                parents.insert( theNode );
                theNode->addChildNode( this );
            }
        }

        /* Anchor member object frame */
        memberObject->members.setOwner( this );

        /* Set value and stored value */
        touched     = false;
        changed     = false;
        value       = memberObject->cloneWithoutConnections();
        storedValue = NULL;

        /* Anchor value member object frame */
        static_cast<MemberObject*>( value )->members.setOwner( this );
    }

    return (*this);
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
        copyMembers[i].replaceDAGVariable( theMemberClone );

        copy->parents.insert( theMemberClone );
        theMemberClone->addChildNode( copy );
    }
    if ( memberObject->isType( List_name ) ) {
        
        const ArgumentFrame& elements     = static_cast<List*>( memberObject )->getElements();
        ArgumentFrame&       copyElements = const_cast<ArgumentFrame&>( static_cast<List*>( copy->memberObject )->getElements() );
        for ( size_t i = 0; i < elements.size(); i++ ) {

            DAGNode* theElementClone = elements[i].getVariable()->cloneDAG( newNodes );
            copyElements[i].replaceDAGVariable( theElementClone );

            copy->parents.insert( theElementClone );
            theElementClone->addChildNode( copy );
        }
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Execute function to get its value */
DAGNode* MemberNode::executeMethod(const std::string& name, const std::vector<Argument>& args) const {

    /* Using this calling convention indicates that we are only interested in
       evaluating the function once */
    return memberObject->executeMethod(name, args);
}


/** Get class vector describing type of DAG node */
const VectorString& MemberNode::getDAGClass() const {

    static VectorString rbClass = VectorString( MemberNode_name ) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Get element for parser */
DAGNode* MemberNode::getElement( VectorIndex& index ) {

    if ( index.size() == 0 )
        return this;
    
    return memberObject->getElement( index );
}


/** Get element owner for parser */
DAGNode* MemberNode::getElementOwner( VectorIndex& index ) {

    // Check for erroneous call
    if ( index.size() == 0 )
        throw RbException( "Unexpected call to getElementOwner with empty index" );

    // Return this if we are the owner
    if ( index.size() == 1 ) {

        if ( index[0]->isType( Integer_name) ) {
            
            int i = static_cast<const Integer*>( index[0] )->getValue();
            
            if ( i < 0 )
                throw RbException( "Unexpected call to getElementOwner with terminal empty index" );
            
            if ( i >= static_cast<int>( memberObject->getElementsSize() ) )
                throw RbException ( "Index out of bound for " + getName() + index.toIndexString() );                
        }

        return this;
    }
    
    // Pop one index and delegate to element if allowed by member object
    DAGNode* element;
    if ( index[0]->isType( Integer_name ) ) {
 
        int i = static_cast<const Integer*>( index[0] )->getValue();

        // Negative index must be handled by this object.
        if ( i < 0 )
            return this;

        element = memberObject->getElement( size_t( i ) );
    }
    else {
        
        std::string s = static_cast<const RbString*>( index[0] )->getValue();
        
        element = memberObject->getElement( s );
    }
    
    if ( element->getSlot() == NULL ) {

        // Not permitted by the member object
        delete element;
        return this;
    }
    else {

        // Permitted by the member object
        index.pop_front();
        return element->getElementOwner( index );
    }
}


/** Get type of a named member variable */
const TypeSpec& MemberNode::getMemberTypeSpec( const RbString& name ) const {

    return memberObject->getMemberTypeSpec( name );
}


/** Is node a constant value? */
bool MemberNode::isConst( void ) const {

    if ( memberObject == NULL )
        return true;
    else
        return memberObject->isConstant();
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


/** Set element for parser, if our member object wants this (see getElementOwner) */
void MemberNode::setElement( size_t index, DAGNode* var, bool convert ) {

    if ( index >= memberObject->getElementsSize() ) {
        std::ostringstream msg;
        msg << "Index out of bound for " << getName() << index;
        throw RbException( msg );
    }

    // Invalidate current value and then set element
    touched = true;
    changed = false;
    memberObject->setElement( index, var, convert );
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void MemberNode::update(void) {

    if (touched && !changed) {

        assert( storedValue == NULL );

        storedValue = value;
        value       = memberObject->cloneWithoutConnections();
        changed     = true;
    }
}

