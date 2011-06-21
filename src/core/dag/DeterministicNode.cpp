/**
 * @file
 * This file contains the implementation of some function in DeterministicNode,
 * which is an abstract base class for DAG nodes associated with expressions
 * (equations) determining their value.
 *
 * @brief Partial implementation of DeterministicNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-30 00:19:25 +0100 (Ons, 30 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id: DeterministicNode.cpp 216 2009-12-29 23:19:25Z ronquist $
 */

#include "ArgumentRule.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbNames.h"
#include "UserInterface.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>


/** Constructor of empty deterministic node */
DeterministicNode::DeterministicNode( const std::string& valType ) :
    VariableNode( valType ), changed( false ) {
}


/** Destructor */
DeterministicNode::~DeterministicNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete DeterministicNode with references" );
}


/** Get affected nodes: touch and pass through to next stochastic node */
void DeterministicNode::getAffected( std::set<StochasticNode*>& affected ) {

    /* If we have already touched this node, we are done; otherwise, get the affected children */
    if ( !touched ) {
        touched = true;
        changed = false;
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->getAffected( affected );
        }
    }
}


/** Get class vector describing type of DAG node */
const VectorString& DeterministicNode::getDAGClass() const {

    static VectorString rbClass = VectorString( DeterministicNode_name ) + VariableNode::getDAGClass();
    return rbClass;
}


/* Get default moves, if any */
MoveSchedule* DeterministicNode::getDefaultMoves( void ) {

    const RbComplex* complexValue = dynamic_cast<const RbComplex*>( value );
    if ( complexValue != NULL )
        return complexValue->getDefaultMoves( this );
    else
        return NULL;
}


/** Get stored value */
const RbObject* DeterministicNode::getStoredValue( void ) {

    if ( !touched )
        return value;

    if ( !changed )
        update();
    
    return storedValue;
}


/** Get value */
const RbObject* DeterministicNode::getValue( void ) {

    if ( touched && !changed )
        update();

    return value;
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keep( void ) {

    keepAffected();
}


/** Keep value of node and affected variable nodes */
void DeterministicNode::keepAffected( void ) {

    if ( touched ) {
        if ( !changed )
            update();
        if ( storedValue ) {
            delete storedValue;
            storedValue = NULL;
        }
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->keepAffected();
        }
    }
    touched = changed = false;
}


/** Print value for user */
void DeterministicNode::printValue( std::ostream& o ) {

    if ( touched && !changed )
        update();

    value->printValue(o);
}


/** Restore value of node and affected variable nodes */
void DeterministicNode::restoreAffected( void ) {

    if ( touched ) {
        if ( changed ) {
         	delete value;
            value       = storedValue;
            storedValue = NULL;
        }
        for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ ) {
            (*i)->restoreAffected();
        }
    }
    touched = changed = false;
}


/** Swap parent node */
void DeterministicNode::swapParentNode( DAGNode* oldNode, DAGNode* newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );

    oldNode->removeChildNode( this );
    newNode->addChildNode   ( this );
    parents.erase ( oldNode );
    parents.insert( newNode );

    touched = true;
    changed = false;
    touchAffected();
}


/** Tell affected nodes that upstream value has been reset */
void DeterministicNode::touchAffected( void ) {

    touched = true;
    changed = false;
    for ( std::set<VariableNode*>::iterator i = children.begin(); i != children.end(); i++ )
        (*i)->touchAffected();
}

