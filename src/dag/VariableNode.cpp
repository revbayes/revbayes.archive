/**
 * @file
 * This file contains the implementation of VariableNode, which is the base
 * class for variable nodes in the model DAG.
 *
 * @brief Implementation of VariableNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-13 15:50:43 +0100 (Sön, 13 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 *
 * $Id: DAGNode.cpp 188 2009-12-13 14:50:43Z ronquist $
 */

#include "Monitor.h"
#include "Move.h"
#include "MoveSchedule.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "VectorInteger.h"
#include "VectorString.h"
#include "VariableNode.h"

#include <algorithm>


/** Constructor */
VariableNode::VariableNode( const std::string& valType )
    : DAGNode( valType ), touched( false ), storedValue( NULL ) {
}


/** Destructor */
VariableNode::~VariableNode( void ) {

    if ( numRefs() != 0 )
        throw RbException( "Cannot delete VariableNode with references" );

    if ( storedValue )
        delete storedValue;

    /* Remove this node as a child node of parents and delete these if appropriate */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ ) {
        (*i)->removeChildNode( this );
        if ( (*i)->isTemp() )
            delete (*i);
    }
}


/** Get class vector describing type of DAG node */
const VectorString& VariableNode::getDAGClass() const {

    static VectorString rbClass = VectorString( VariableNode_name ) + DAGNode::getDAGClass();
    return rbClass;
}


/** Get default monitors */
std::vector<Monitor*> VariableNode::getDefaultMonitors( void ) {

    std::vector<Monitor*>   monitors;
    monitors.push_back( new Monitor( this, 100 ) );

    return monitors;
}


/** Is it possible to change parent node oldNode to newNode? */
bool VariableNode::isParentMutableTo( const DAGNode* oldNode, const DAGNode* newNode ) const {

    // Find node among parents
    if ( parents.find( const_cast<DAGNode*>( oldNode ) ) == parents.end() )
        throw RbException( "Node is not a parent" );
   
    // If all parents are in variable slots, answer is yes (given that the parent node itself is mutable)
    return true;
}

