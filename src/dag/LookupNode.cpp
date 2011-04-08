/**
 * @file
 * This file contains the implementation of LookupNode, which is used to
 * hold and perform lookups of variables.
 *
 * @brief Implementation of LookupNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: LookupNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "Boolean.h"
#include "DAGNode.h"
#include "Integer.h"
#include "LookupNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty lookup node */
LookupNode::LookupNode( const std::string& valType )
    : DeterministicNode( valType ), baseVariable( NULL ), baseLookup( NULL ),
      memberName(), indexArgs(), valueDim( 0 ) {
}


/** Construct lookup of regular variable */
LookupNode::LookupNode( DAGNode* var, IndexArgs& indxArgs )
    : DeterministicNode( var->getValueType() ), baseVariable( var ), baseLookup( NULL ),
      memberName(), indexArgs( indxArgs ), valueDim( 0 ) {

    /* Check index arguments */
    for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) == NULL )
            continue;
        if ( (*i) != NULL && 
             !Workspace::userWorkspace().isXConvertibleToY( (*i)->getTypeSpec(), TypeSpec( Integer_name, 0 ) ) &&
             !Workspace::userWorkspace().isXConvertibleToY( (*i)->getTypeSpec(), TypeSpec( RbString_name, 0 ) ) )
            throw RbException( "Invalid type of index argument" );
    }

    /* Update dimension variable to reflect result of lookup */
    valueDim -= indexArgs.size();
        
    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( IndexArgs::const_iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL && (*i)->isParentInDAG(this, done) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    if ( baseVariable->isParentInDAG(this, done) )
        throw RbException( "Invalid assignment: cycles in the DAG" );

    /* Set parents and add this node as a child node of these */
    for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL ) {
            parents.insert( *i );
            (*i)->addChildNode( this );
        }
    }
    parents.insert( baseVariable );
    baseVariable->addChildNode( this );

    /* Set value and stored value */
    touched     = false;
    changed     = false;
    value       = lookup()->getValue()->clone();
    storedValue = NULL;
}


/** Construct lookup of member variable */
LookupNode::LookupNode( LookupNode* lookupNode, RbString* membrName, IndexArgs&  indxArgs )
    : DeterministicNode( lookupNode->getMemberTypeSpec( *membrName ).getType() ), baseVariable( NULL ), baseLookup( lookupNode ),
      memberName( membrName ), indexArgs( indxArgs ), valueDim( 0 ) {

    /* Check index arguments */
    for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) == NULL )
            continue;
        if ( (*i) != NULL && 
             !Workspace::userWorkspace().isXConvertibleToY( (*i)->getTypeSpec(), TypeSpec( Integer_name, 0 ) ) &&
             !Workspace::userWorkspace().isXConvertibleToY( (*i)->getTypeSpec(), TypeSpec( RbString_name, 0 ) ) )
            throw RbException( "Invalid type of index argument" );
    }
       
    /* Update dimension variable to reflect result of lookup */
    valueDim = baseLookup->getMemberTypeSpec( *memberName ).getDim() - indexArgs.size();

    /* Check for cycles */
    std::list<DAGNode*> done;
    for ( IndexArgs::const_iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL && (*i)->isParentInDAG( this, done ) )
            throw RbException( "Invalid assignment: cycles in the DAG" );
    }
    if ( baseLookup->isParentInDAG( this, done ) )
        throw RbException( "Invalid assignment: cycles in the DAG" );

    /* Set parents and add this node as a child node of these */
    for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL ) {
            parents.insert( *i );
            (*i)->addChildNode( this );
        }
    }
    parents.insert( baseLookup );
    baseLookup->addChildNode( this );

    /* Set value and stored value */
    DAGNode* temp = lookup();
    value         = temp->getValue()->clone();
    storedValue   = NULL;
    delete temp;
}


/** Copy constructor */
LookupNode::LookupNode( const LookupNode& x ) : DeterministicNode( x ) {

    baseVariable   = x.baseVariable;
    baseLookup     = x.baseLookup;
    memberName     = x.memberName->clone();
    indexArgs      = x.indexArgs;
    touched        = x.touched;
    changed        = x.changed;
    value          = x.value->clone();
    if ( x.storedValue != NULL )
        storedValue = x.storedValue->clone();
    else
        storedValue    = NULL;

    /* Set parents and add this node as a child node of these */
    for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL ) {
            parents.insert( *i );
            (*i)->addChildNode( this );
        }
    }
    if ( baseVariable != NULL ) {
        parents.insert( baseVariable );
        baseVariable->addChildNode( this );
    }
    if ( baseLookup != NULL ) {
        parents.insert( baseLookup );
        baseLookup->addChildNode( this );
    }
}


/**
 * Destructor
 *
 * @note Parent nodes, including baseVariable or baseLookup, destroyed
 *       by VariableNode destructor
 */
LookupNode::~LookupNode( void ) {

    if ( numRefs() != 0 )
        throw RbException( "Cannot delete LookupNode with references" );

    delete memberName;
}


/** Assignment operator */
LookupNode& LookupNode::operator=(const LookupNode& x) {

    if (this != &x) {

        if ( valueType != x.valueType || getDim() != x.getDim() )
            throw RbException( "Type mismatch in lookup node assignment" );
        
        touched     = false;
        changed     = false;
        delete value;
        if ( storedValue )
            delete storedValue;
        delete memberName;

        /* Remove this node as a child node of parents */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ ) {
            (*i)->removeChildNode( this );
            if ( (*i)->numRefs() == 0 )
                delete (*i);
        }
        parents.clear();

        baseVariable    = x.baseVariable;
        baseLookup      = x.baseLookup;
        memberName      = x.memberName->clone();
        indexArgs       = x.indexArgs;
        touched         = x.touched;
        changed         = x.changed;
        value           = x.value->clone();
        if ( x.storedValue != NULL )
            storedValue = x.storedValue->clone();
        else
            storedValue = NULL;

        /* Set parents and add this node as a child node of these */
        for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
            if ( (*i) != NULL ) {
                parents.insert( *i );
                (*i)->addChildNode(this);
            }
        }
        if ( baseVariable != NULL ) {
            parents.insert( baseVariable );
            baseVariable->addChildNode( this );
        }
        if ( baseLookup != NULL ) {
            parents.insert( baseLookup );
            baseLookup->addChildNode( this );
        }
    }

    return ( *this );
}


/** Clone this object */
LookupNode* LookupNode::clone( void ) const {

	return new LookupNode( *this );
}


/** Clone the entire graph: clone children, swap parents */
LookupNode* LookupNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<LookupNode*>( newNodes[this ] );

    /* Get pristine copy */
    LookupNode* copy = new LookupNode( valueType );
    newNodes[ this ] = copy;

    /* Clone parents */
    copy->memberName = new RbString( *memberName );
    for ( std::vector<DAGNode*>::const_iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
        if ( (*i) != NULL ) {
            DAGNode* theParentClone = (*i)->cloneDAG( newNodes );
            copy->indexArgs.push_back( theParentClone );
            copy->parents.insert( theParentClone );
            theParentClone->addChildNode( copy );
        }
        else
            copy->indexArgs.push_back( NULL );
    }
    if ( baseVariable != NULL ) {
        copy->baseVariable = baseVariable->cloneDAG( newNodes );
        copy->parents.insert( copy->baseVariable );
        copy->baseVariable->addChildNode( copy );
    }
    if ( baseLookup != NULL ) {
        copy->baseLookup = baseLookup->cloneDAG( newNodes );
        copy->parents.insert( copy->baseLookup );
        copy->baseLookup->addChildNode( copy );
    }

    copy->value        = value->clone();
    if ( storedValue != NULL )
        copy->storedValue = storedValue->clone();
    else
        copy->storedValue  = NULL;
    copy->touched      = touched;
    copy->changed      = changed;

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Does element exist ? */
bool LookupNode::existsElement( VectorInteger& index ) {

    return getReference()->existsElement( index );
}


/** Get class vector describing type of DAG node */
const VectorString& LookupNode::getDAGClass() const {

    static VectorString rbClass = VectorString( LookupNode_name ) + VariableNode::getDAGClass();
    return rbClass;
}


/** Get element for parser */
DAGNode* LookupNode::getElement( VectorInteger& index ) {

    return getReference()->getElement( index );
}


/** Get element reference for modification by parser */
DAGNode* LookupNode::getElementRef( VectorNatural& index) {

    return getReference()->getElementRef( index );
}


/** Get type of a named member variable */
const TypeSpec& LookupNode::getMemberTypeSpec( const RbString& name ) const {

    if ( baseLookup == NULL )
        throw RbException( "Not a member variable" );

    const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseLookup->getValue() );
    if ( theBaseObject == NULL )
        throw RbException( "Base variable does not have members");

    return theBaseObject->getMemberTypeSpec( name );
}


/** Is it possible to change parent node oldNode to newNode? */
bool LookupNode::isParentMutableTo( const DAGNode* oldNode, const DAGNode* newNode ) const {

    // First find node among parents
    if ( parents.find( const_cast<DAGNode*>( oldNode ) ) == parents.end() )
        throw RbException( "Node is not a parent" );

    // Now find node among indexArgs or variable/baseVariable
    if ( oldNode == baseVariable ) {
        if ( Workspace::globalWorkspace().isXConvertibleToY(newNode->getTypeSpec(), baseVariable->getTypeSpec()) )
            return true;
        else
            return false;
    }
    else if ( oldNode == baseLookup ) {
        if ( Workspace::globalWorkspace().isXConvertibleToY(newNode->getTypeSpec(), baseLookup->getTypeSpec()) )
            return true;
        else
            return false;
    }
    else {
        std::vector<DAGNode*>::const_iterator it = std::find( indexArgs.begin(), indexArgs.end(), oldNode );
        if ( it == indexArgs.end() )
            throw RbException( "Node is not a parameter" );
        if ( Workspace::globalWorkspace().isXConvertibleToY( newNode->getTypeSpec(), TypeSpec( Integer_name , 0 ) ) ||
             Workspace::globalWorkspace().isXConvertibleToY( newNode->getTypeSpec(), TypeSpec( RbString_name, 0 ) ) )
            return true;
        else
            return false;
    }

    return false;
}


/** Look up the variable */
DAGNode* LookupNode::lookup( void ) {
    
    // Invalidate current value
    touched = true;
    changed = false;

    // Look up variable
    if ( indexArgs.size() == 0 ) {
        if ( baseVariable != NULL )
            return baseVariable;    // This should never be the case if we are constructed correctly
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseLookup->getValue() );
            if ( theBaseObject == NULL )
                throw RbException( "Base variable does not have members");

            return const_cast<DAGNode*>( theBaseObject->getVariable( *memberName ) );
        }
    }
    else {
        // Get the index vector from the index arguments
        //! @todo Support string indices
        VectorInteger index;
        for ( IndexArgs::iterator i = indexArgs.begin(); i != indexArgs.end(); i++ ) {
            if ( (*i) == NULL )
                index.push_back( -1 );
            else
                index.push_back( ( static_cast<const Integer*>( (*i)->getValue() ) )->getValue() - 1 );
        }

        if ( baseVariable != NULL )
            return baseVariable->getElement( index );
        else {
            const MemberObject* theBaseObject = dynamic_cast<const MemberObject*>( baseLookup->getValue() );
            if ( theBaseObject == NULL )
                throw RbException( "Base variable does not have members" );

            return const_cast<DAGNode*>( theBaseObject->getVariable( *memberName ) )->getElement( index );
        }
    }
}


/** Print struct for user (a lookup node should be impossible to access for user) */
void LookupNode::printStruct(std::ostream& o) const {

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
std::string LookupNode::richInfo( void ) const {

    std::ostringstream o;

    o << "LookupNode:" << std::endl;

    o << "name         = " << getName() << std::endl;
    o << "touched      = " << ( touched ? "true" : "false" ) << std::endl;
    o << "changed      = " << ( changed ? "true" : "false" ) << std::endl;

    if ( baseVariable != NULL ) {
        o << "baseVariable =" << std::endl;
        baseVariable->printValue( o );
        o << std::endl;
    }
    if ( baseLookup != NULL ) {
        o << "baseLookup =" << std::endl;
        baseLookup->printValue( o );
        o << std::endl;
        o << "memberName = " << *memberName << std::endl;
    }

    o << "value      = ";
    value->printValue( o );
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue( o );
    o << std::endl;

    return o.str();
}


/** Allow parser to set element */
void LookupNode::setElement( VectorNatural& index, DAGNode* var ) {

    getReference()->setElement( index, var );
}


/** Swap parent node both in parents set and in processed arguments */
void LookupNode::swapParentNode( DAGNode* oldNode, DAGNode* newNode ) {

    /* First swap the node in the parents set */
    if ( parents.find(oldNode) == parents.end() )
        throw RbException( "Node is not a parent" );
    oldNode->removeChildNode( this );
    newNode->addChildNode( this );
    parents.erase( oldNode );
    parents.insert( newNode );

    /* Now find and swap the node among parameters */
    if ( oldNode == baseVariable ) {
        baseVariable = newNode;
    }
    else if ( oldNode == baseLookup ) {
        baseLookup = static_cast<LookupNode *>( newNode );
    }
    else {
        std::vector<DAGNode*>::iterator it = std::find( indexArgs.begin(), indexArgs.end(), oldNode );
        if ( it == indexArgs.end() )
            throw RbException( "Node is not a parameter" );
        it = indexArgs.erase( it );
        indexArgs.insert( it, newNode );
    }

    touched = true;
    changed = false;
    touchAffected();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void LookupNode::update(void) {

    if ( touched && !changed ) {

        assert( storedValue == NULL );

        DAGNode* temp = lookup();

        storedValue   = value;
        value         = temp->getValue()->clone();
        changed       = true;
        
        delete temp;
    }
}

