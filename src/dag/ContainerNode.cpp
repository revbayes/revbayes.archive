/**
 * @file
 * This file contains the implementation of ContainerNode, which
 * is used to hold vectors or multidimensional arrays of DAG nodes.
 *
 * @brief Implementation of ContainerNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-05, version 1.0
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "Container.h"
#include "ContainerNode.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "VariableContainer.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>


/** Construct empty container node */
ContainerNode::ContainerNode( const std::string& valType, int dim )
    : DeterministicNode( valType ), container( NULL ), valueDim( dim ) {
}


/** Construct container node from container */
ContainerNode::ContainerNode( Container* val )
    : DeterministicNode( val->getElementType() ), valueDim( val->getDim() ), container( NULL ) {

    if ( val->isType( VariableContainer_name ) ) { 

        /* Check for cycles */
        container = static_cast<VariableContainer*>( val );
        std::list<DAGNode*> done;
        for ( size_t i = 0; i < container->size(); i++ ) {
            if ( (*container)[i]->isParentInDAG( this, done ) )
                throw RbException ( "Invalid assignment: cycles in the DAG" );
        }

        /* Set parents and add this node as a child */
        for ( size_t i = 0; i < container->size(); i++ ) {
            DAGNode* theNode = (*container)[i];
            parents.insert( theNode );
            theNode->addChildNode(this);
        }

        /* Set value and stored value */
        value       = container->getConstValue();
        storedValue = NULL;
    }
    else {

        // We have a value container
        value       = val;
        storedValue = NULL;
    }
}


/** Copy constructor needed to make sure containers are independent */
ContainerNode::ContainerNode( const ContainerNode& x )
    : DeterministicNode( x ) {

    if ( x.container == NULL ) {

        container = NULL;
    }
    else {

        container = x.container->clone();
    
        /* Set parents and add this node as a child */
        for ( size_t i = 0; i < container->size(); i++ ) {
            DAGNode* theNode = (*container)[i];
            parents.insert( theNode );
            theNode->addChildNode(this);
        }
    }

    value       = x.value->clone();
    storedValue = NULL;
    touched     = false;
    changed     = false;
}


/** Destructor */
ContainerNode::~ContainerNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete ContainerNode with references" ); 

    /* Remove parents first so that DeterministicNode destructor does not get in the way */
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
        (*i)->removeChildNode(this);
    parents.clear();

    if ( container != NULL )
        delete container;    // This will delete any DAG nodes that need to be deleted
}


/** Assignment operator; make sure we get independent nodes */
ContainerNode& ContainerNode::operator=( const ContainerNode& x ) {

    if (this != &x) {

        if ( getTypeSpec() != x.getTypeSpec() )
            throw RbException( "Type mismatch in container variable assignment" );

        /* Remove parents first */
        for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
            (*i)->removeChildNode(this);
        parents.clear();

        delete container;    // This will delete any DAG nodes that need to be deleted

        delete value;
        if (storedValue)
            delete storedValue;

        if ( x.container == NULL ) {

            container = NULL;
        }
        else {

            container = x.container->clone();
        
            /* Set parents and add this node as a child */
            for ( size_t i = 0; i < container->size(); i++ ) {
                DAGNode* theNode = (*container)[i];
                parents.insert( theNode );
                theNode->addChildNode(this);
            }
        }

        value       = x.value->clone();
        storedValue = NULL;
        touched     = false;
        changed     = false;
    }

    return ( *this );
}


/** Clone object */
ContainerNode* ContainerNode::clone() const {

    return new ContainerNode( *this );
}


/** Clone entire graph */
ContainerNode* ContainerNode::cloneDAG(std::map<DAGNode*, DAGNode*>& newNodes) const {

    if ( newNodes.find( (ContainerNode*)(this) ) != newNodes.end() )
        return (ContainerNode*)( newNodes[ (DAGNode*)(this) ] );

    /* Make pristine copy */
    ContainerNode* copy = new ContainerNode( valueType, valueDim );
    newNodes[ (DAGNode*)(this) ] = copy;

    copy->touched     = true;
    copy->changed     = false;
    copy->value       = value->clone();
    copy->storedValue = NULL;

    /* Clone parents through container */
    copy->container = container->clone();   // We get independent copies here, which is not right
    copy->container->clear();
    for ( size_t i=0; i<container->size(); i++ ) {
        DAGNode* theParentCopy = (*container)[i]->cloneDAG(newNodes);
        (*copy->container)[i] = theParentCopy;
        copy->parents.insert( theParentCopy );
        theParentCopy->addChildNode(copy);
    }

    copy->update();
    copy->keepAffected();

    /* Make sure the children clone themselves */
    for(std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        (*i)->cloneDAG(newNodes);
    }

    return copy;
}


/** Does element referred to by index exist? */
bool ContainerNode::existsElement( VectorInteger& index ) {

    // Check for silly references to ourself
    if ( index.size() == 0 )
        return true;

    if ( index.size() > size_t( getDim() ) ) {
        
        // If value container, the element does not exist
        if ( container == NULL )
            return false;

        // Check that references that go beyond the container
        // actually point to an element of the container
        for ( size_t i = 0; i < size_t( getDim() ) ; i++ ) {
            if ( index[i] < 0 )
                return false;
        }

        // Pop off container index and delegate to container element
        VectorInteger containerIndex;
        for ( size_t i = 0; i < size_t( getDim() ); i++ )
            containerIndex = index[i];
        for ( size_t i = 0; i < size_t( getDim() ); i++ )
            index.pop_front();

        return container->getElement( containerIndex )->existsElement( index );
    }

    // If all indices are negative or within bounds, we should be safe
    std::vector<size_t> len;
    if ( container != NULL )
        len = container->getLength();
    else
        static_cast<ValueContainer*>( value )->getLength();

    for ( size_t i = 0; i < index.size(); i++ )
        if ( size_t( index[i]) >= len[i] )
            return false;

    return true;
}


/** Get class vector describing type of DAG node */
const VectorString& ContainerNode::getDAGClass(void) const {

    static VectorString rbClass = VectorString(ContainerNode_name) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Convenient vector access */
DAGNode* ContainerNode::getElement( size_t i ) {

    return getElement( VectorInteger( i ) );
}


/** Get element for parser */
DAGNode* ContainerNode::getElement( VectorInteger& index ) {

    // Check for silly references to ourself
    if ( index.size() == 0 )
        return this;

    if ( index.size() > size_t( getDim() ) ) {
        
        // If value container, the element does not exist
        if ( container == NULL )
            throw RbException( getName() + index.toIndexString() + " does not exist" );

        // Check that references that go beyond the container
        // actually point to an element of the container
        for ( size_t i = 0; i < size_t( getDim() ); i++ ) {
            if ( index[i] < 0 )
                throw RbException( getName() + index.toIndexString() + " is not a valid container" );
        }

        // Delegate to container element
        return container->getElement( index );
    }

    if ( container != NULL )
        return container->getElement( index );
    else
        return static_cast<ValueContainer*>( value )->getElement( index );
}


/**
 * Get element reference for parser, so that it can set the element value or
 * mutate it. This type of request cannot be addressed by a value container,
 * so we need to change ourselves into a variable container if we are currently
 * a constant value container.
 */
DAGNode* ContainerNode::getElementRef( VectorNatural& index ) {

    // Invalidate current value, since it is likely to change
    touched = true;
    changed = false;

    // Check for silly references to ourself
    if ( index.size() == 0 )
        return this;

    if ( index.size() < size_t( getDim() ) )
            throw RbException( getName() + index.toIndexString() + " is a temp variable" );

    if ( index.size() > size_t( getDim() ) ) {
        
        // If value container, the element does not exist
        if ( container == NULL )
            throw RbException( getName() + index.toIndexString() + " does not exist" );

        // Delegate to container element
        touched = true;
        changed = false;
        VectorInteger containerIndex;
        for ( size_t i = 0; i < size_t( getDim() ); i++ )
            containerIndex.push_back( index[i] );
        for ( size_t i = 0; i < size_t( getDim() ); i++)
            index.pop_front();

        return container->getElement( containerIndex )->getElementRef( index );
    }

    if ( container != NULL ) {
        VectorInteger containerIndex = index;
        index.clear();
        return container->getElement( containerIndex );
    }

    // We transform ourselves into a variable container
    container = new VariableContainer( static_cast<ValueContainer&>( *value ) );
    touched = true;
    changed = false;
    VectorInteger containerIndex = index;
    index.clear();
    return container->getElement( containerIndex );
}


/** Get index of specified element */
VectorNatural ContainerNode::getIndex(const DAGNode* element) const {

    if ( container == NULL )
        throw RbException( element->getName() + " is not an element of " + getName() );
    
    return container->getIndex( element );
}


/** Is it possible to mutate to newNode? */
bool ContainerNode::isMutableTo( const DAGNode* newNode ) const {

    return false;
}


/** Is it possible to change parent node oldNode to newNode? */
bool ContainerNode::isParentMutableTo( const DAGNode* oldNode, const DAGNode* newNode ) const {

    // Check that all parents except the caller are convertible to the new type and dim
    for (std::set<DAGNode*>::const_iterator i=parents.begin(); i!=parents.end(); i++) {
        if ( (*i) != NULL && (*i) != const_cast<DAGNode*>(oldNode) ) {
            if ( !(*i)->getValue()->isConvertibleTo( TypeSpec(newNode->getValueType(), newNode->getDim()), false ) )
                return false;
        }
    }
    
    // Check that all children allow this node to permute to the new type and dim
    ContainerNode* temp = new ContainerNode( newNode->getValueType(), valueDim );
    for (std::set<VariableNode*>::const_iterator i=children.begin(); i!=children.end(); i++) {
        if ( !(*i)->isParentMutableTo(this, temp) ) {
            delete temp;
            return false;
        }
    }
    delete temp;

    return true;
}


/** Mutate to newNode */
void ContainerNode::mutateTo( DAGNode* newNode ) {
    
    throw RbException("Not implemented yet");
    
}


/** Print struct for user */
void ContainerNode::printStruct( std::ostream& o ) const {

    if (touched && !changed)
        throw RbException ("Cannot print value while in touched state");

    o << "Wrapper:" << std::endl;
    o << "$class  = " << getDAGClass() << std::endl;
    o << "$dim    = " << getDim() << std::endl;
    o << "$length = " << VectorInteger(container->getLength()) << std::endl;
    o << "$value  = " << value << std::endl;
    o << "$parents = " << std::endl;
    printParents(o);
    o << std::endl;
    o << "$children = " << std::endl;
    printChildren(o);
    o << std::endl;
    o << std::endl;
}


/** Get complete info about object */
std::string ContainerNode::richInfo(void) const {

    std::ostringstream o;

    o << "DAGNode plate:" << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << VectorInteger(container->getLength()) << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "container   = " << std::endl;
    o << *container << std::endl;
    o << "value       = " << std::endl;
    o << value << std::endl;
    o << "storedValue = " << std::endl;
    o << storedValue;

    return o.str();
}


/** Get total number of elements of container */
size_t ContainerNode::size( void ) const {

    if ( container != NULL )
        return container->size();
    else
        return static_cast<Container*>( value )->size();
}


/** Swap parent node in container and parents */
void ContainerNode::swapParentNode( DAGNode* oldNode, DAGNode* newNode ) {

    if ( parents.find( oldNode ) == parents.end() )
        throw RbException( "Node is not a parent" );
    oldNode->removeChildNode( this );
    newNode->addChildNode( this );
    parents.erase( oldNode );
    parents.insert( newNode );

    std::vector<DAGNode*>::iterator it = std::find( container->elements.begin(), container->elements.end(), oldNode );
    if ( it == container->elements.end() )
        throw RbException( "Node is not in container" );
    it = container->elements.erase( it );
    container->elements.insert( it, newNode );

    touched = true;
    changed = false;
    touchAffected();
}


/** Update value and stored value after node and its surroundings have been touched by a move */
void ContainerNode::update(void) {

    if (touched && !changed) {
        if (storedValue != NULL)
            delete storedValue;
        storedValue = value;
        value = container->getConstValue();
        changed = true;
    }
}

