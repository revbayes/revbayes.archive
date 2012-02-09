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
#include "RbUtil.h"
#include "StochasticNode.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cassert>
#include <iostream>
#include <string>
#include <set>


// Definition of the static type spec member
const TypeSpec ConstantNode::typeSpec(ConstantNode_name);

/** Constructor from value */
ConstantNode::ConstantNode( RbLanguageObject* val ) : DAGNode( val ) {

}


/** Constructor from value class */
ConstantNode::ConstantNode( const std::string& valType ) : DAGNode( valType ) {

}

/** Copy constructor */
ConstantNode::ConstantNode( const ConstantNode &x ) : DAGNode( x ) {
    
}


/** Clone this object */
ConstantNode* ConstantNode::clone( void ) const {

    return new ConstantNode( *this );
}


/** Cloning the entire graph only involves children for a constant node */
RbPtr<DAGNode> ConstantNode::cloneDAG( std::map<const DAGNode*, RbPtr<DAGNode> >& newNodes ) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return ( newNodes[ this ] );

    /* Make pristine copy */
    ConstantNode* copy = clone();
    newNodes[ this ] = copy;
    
    /* Make sure the children clone themselves */
    for( std::set<VariableNode* >::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }
 
    return copy;
}


/** 
 * Get the affected nodes.
 * This call is started by the parent and since we don't have one this is a dummy implementation!
 */
void ConstantNode::getAffected(std::set<StochasticNode* > &affected) {
    // do nothing
    throw RbException("You should never call getAffected() of a constant node!!!");
}


/** Get class vector describing type of DAG node */
const VectorString& ConstantNode::getClass() const {

    static VectorString rbClass = VectorString( ConstantNode_name ) + DAGNode::getClass();
    return rbClass;
}


const RbLanguageObject& ConstantNode::getStoredValue(void) const {
    return *value;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ConstantNode::getTypeSpec(void) const {
    return typeSpec;
}


const RbLanguageObject& ConstantNode::getValue(void) const {
    return *value;
}


RbLanguageObject& ConstantNode::getValue(void) {
    return *value;
}


const RbLanguageObject* ConstantNode::getValuePtr(void) const {
    return value;
}

/** Keep value of node */
void ConstantNode::keepMe( void ) {
    // do nothing
}

/** Print value for user */
void ConstantNode::printValue( std::ostream& o ) const {

    if ( value != NULL )
        value->printValue(o);
    else  /* NULL value */
        o << "NULL";
}


/** Print struct for user */
void ConstantNode::printStruct(std::ostream &o) const {

    o << "_Class        = " << getClass() << std::endl;
    o << "_valueType    = " << getValueType() << std::endl;
    o << "_value        = ";
    value->printValue(o);
    o << std::endl;

    o << "_children     = ";
    printChildren(o);
    o << std::endl;

    o << std::endl;
}

/** Restore value of node */
void ConstantNode::restoreMe( void ) {
    // do nothing
}


/** Touch value of node */
void ConstantNode::touchMe( void ) {
    // do nothing
}

