/**
 * @file
 * This file contains the implementation of ConstantNode, which is derived
 * from DAGNode. ConstantNode is used for DAG nodes holding constant
 * values other than containers or member objects (which may or may not
 * be constant).
 *
 * @brief Implementation of ConstantNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-11-19 17:29:33 +0100 (Tor, 19 Nov 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends ConstantNode
 *
 * $Id: ConstantNode.h 69 2009-11-19 16:29:33Z ronquist $
 */


#include "ConstantNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <string>
#include <set>


/** Constructor from value */
ConstantNode::ConstantNode( RbObject* val ) : DAGNode( val ) {

    if ( val->getDim() > 0 )
        throw RbException( "Invalid attempt to create ConstantNode with container value" );
}


/** Constructor from value class */
ConstantNode::ConstantNode( const std::string& valType ) : DAGNode( valType ) {

    if ( Workspace::userWorkspace().isXOfTypeY( valType, Container_name ) )
        throw RbException( "Invalid attempt to create ConstantNode with container value" );
}


/** Clone this object */
ConstantNode* ConstantNode::clone( void ) const {

    return new ConstantNode( *this );
}


/** Cloning the entire graph only involves children for a constant node */
ConstantNode* ConstantNode::cloneDAG( std::map<const DAGNode*, DAGNode*>& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<ConstantNode*>( newNodes[ this ] );

    /* Make pristine copy */
    ConstantNode* copy = clone();
    newNodes[ this ] = copy;

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }
 
    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& ConstantNode::getDAGClass() const {

    static VectorString rbClass = VectorString( ConstantNode_name ) + DAGNode::getDAGClass();
    return rbClass;
}


/** Print value for user */
void ConstantNode::printValue( std::ostream& o ) const {

    if ( value )
        value->printValue(o);
    else  /* NULL value */
        o << "NA";
}


/** Print struct for user */
void ConstantNode::printStruct(std::ostream &o) const {

    o << "_DAGClass     = " << getDAGClass() << std::endl;
    o << "_valueType    = " << valueType << std::endl;
    o << "_dim          = " << getDim() << std::endl;
    o << "_value        = " << value->briefInfo() << std::endl;

    o << "_parents      = ";
    printParents(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}


/** Complete info on object */
std::string ConstantNode::richInfo( void ) const {

    std::ostringstream o;
    o << "ConstantNode: value = " << value->briefInfo();

    return o.str();
}

