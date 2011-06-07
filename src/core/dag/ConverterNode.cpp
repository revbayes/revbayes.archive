/**
 * @file
 * This file contains the implementation of ConverterNode.
 *
 * @brief Implementation of ConverterNode
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

#include "Boolean.h"
#include "ConverterNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <algorithm>
#include <cassert>


/** Constructor of pristine converter node */
ConverterNode::ConverterNode(const std::string& type, int dim)
    : DeterministicNode(type), valueDim(dim) {
}


/** Basic constructor of converter node */
ConverterNode::ConverterNode( DAGNode* origNode, const std::string& type, int dim )
    : DeterministicNode( type ), valueDim( dim ) {

    /* Check that the call is reasonable */
    if ( !origNode->getValue()->isConvertibleTo( type, dim, true ) )
        throw RbException( "Invalid type converter node" );
        
    /* Check whether type conversion is safe */
    if ( !origNode->getValue()->isConvertibleTo( type, dim, false ) )
        throw RbException( "Unsafe type conversion in converter node" );

    /* Check for cycles */
    std::list<DAGNode*> done;
    if ( origNode->isParentInDAG( this, done ) )
        throw RbException( "Invalid assignment: cycles in the DAG" );

    /* Set parent and add this node as a child */
    parents.insert( origNode );
    origNode->addChildNode( this );

    /* Set value and stored value */
    touched     = false;
    changed     = false;
    value       = origNode->getValue()->convertTo( valueType, valueDim );
    storedValue = NULL;
}


/** Destructor throws an appropriate message */
ConverterNode::~ConverterNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete ConverterNode with references" );
}


/** Clone this object */
ConverterNode* ConverterNode::clone( void ) const {

	return new ConverterNode( *this );
}


/** Clone the entire graph: clone children, swap parent */
ConverterNode* ConverterNode::cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const {

    if ( newNodes.find( this ) != newNodes.end())
        return static_cast<ConverterNode*>( newNodes[ this ] );

    /* Get pristine copy */
    ConverterNode* copy = new ConverterNode( valueType, valueDim );
    newNodes[ this ] = copy;

    /* Clone parents */
    copy->touched     = touched;
    copy->changed     = changed;
    copy->value       = value->clone();
    if ( storedValue != NULL )
        copy->storedValue = storedValue->clone();
    else
        copy->storedValue = NULL;

    DAGNode* theParentClone = ( *parents.begin() )->cloneDAG( newNodes );
    copy->parents.insert( theParentClone );
    theParentClone->addChildNode( copy );

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
    }

    return copy;
}


/** Get class vector describing type of DAG node */
const VectorString& ConverterNode::getDAGClass() const {

    static VectorString rbClass = VectorString( ConverterNode_name ) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Is it possible to change parent node oldNode to newNode? */
bool ConverterNode::isParentMutableTo(const DAGNode* oldNode, const DAGNode* newNode) const {

    // First find out if node is parent
    if ( *parents.begin() != oldNode )
        throw RbException( "Node is not a parent" );

    // See if the new node value is convertible to the required type and dim
    if ( newNode->getValue()->isConvertibleTo( valueType, valueDim, false ) )
        return true;
    
    return false;
}


/** Print struct for user */
void ConverterNode::printStruct(std::ostream& o) const {

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
std::string ConverterNode::richInfo(void) const {

    std::ostringstream o;

    o << "ConverterNode:" << std::endl;

    o << "name        = " << getName() << std::endl;
    o << "touched     = " << (touched ? "true" : "false") << std::endl;
    o << "changed     = " << (changed ? "true" : "false") << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "valueDim    = " << valueDim << std::endl;

    o << "value       = ";
    value->printValue(o);
    o << std::endl;

    o << "storedValue = ";
    storedValue->printValue(o);
    o << std::endl;

    return o.str();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void ConverterNode::update( void ) {

    if ( touched && !changed ) {
        
        assert( storedValue == NULL );

        storedValue = value;
        value       = (*parents.begin())->getValue()->convertTo(valueType, valueDim);
        changed     = true;
    }
}

