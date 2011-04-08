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


#include "Boolean.h"
#include "ConstantNode.h"
#include "Container.h"
#include "ContainerNode.h"
#include "ConverterNode.h"
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

    touched = false;
    changed = false;
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
            theNode->addChildNode( this );
        }
    }

    value       = x.value->clone();
    if ( x.storedValue != NULL )
        storedValue = x.storedValue->clone();
    else
        storedValue = NULL;
    touched     = false;
    changed     = false;
}


/** Destructor */
ContainerNode::~ContainerNode( void ) {

    if ( numRefs() != 0 )
        throw RbException ( "Cannot delete ContainerNode with references" ); 

    /* Remove parents first so that DeterministicNode destructor does not get in the way */
    for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
        (*i)->removeChildNode( this );
    parents.clear();

    if ( container != NULL )
        delete container;    // This will delete any DAG nodes that need to be deleted
}


/** Assignment operator; make sure we get independent nodes */
ContainerNode& ContainerNode::operator=( const ContainerNode& x ) {

    if ( this != &x ) {

        if ( getTypeSpec() != x.getTypeSpec() )
            throw RbException( "Type mismatch in container variable assignment" );

        /* Remove parents first */
        for ( std::set<DAGNode*>::iterator i = parents.begin(); i != parents.end(); i++ )
            (*i)->removeChildNode( this );
        parents.clear();

        delete container;    // This will delete any DAG nodes that need to be deleted

        delete value;
        if ( storedValue )
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
                theNode->addChildNode( this );
            }
        }

        value       = x.value->clone();
        if ( x.storedValue != NULL )
            storedValue = x.storedValue->clone();
        else
            storedValue = NULL;
        touched     = x.touched;
        changed     = x.changed;
    }

    return ( *this );
}


/** Clone object */
ContainerNode* ContainerNode::clone() const {

    return new ContainerNode( *this );
}


/** Clone entire graph */
ContainerNode* ContainerNode::cloneDAG(std::map<const DAGNode*, DAGNode*>& newNodes) const {

    if ( newNodes.find( this ) != newNodes.end() )
        return static_cast<ContainerNode*>( newNodes[ this ] );

    /* Make pristine copy */
    ContainerNode* copy = new ContainerNode( valueType, valueDim );
    newNodes[ this ] = copy;

    copy->touched     = touched;
    copy->changed     = changed;
    copy->value       = value->clone();
    if ( storedValue != NULL )
        copy->storedValue = storedValue->clone();
    else
        copy->storedValue = NULL;

    /* Clone parents through container */
    copy->container = container->clone();   // We get independent copies here, which is not right
    copy->container->clear();
    for ( size_t i = 0; i < container->size(); i++ ) {

        DAGNode* theParentClone = (*container)[i]->cloneDAG(newNodes);
        (*copy->container)[i] = theParentClone;

        copy->parents.insert( theParentClone );
        theParentClone->addChildNode( copy );
    }

    /* Make sure the children clone themselves */
    for( std::set<VariableNode*>::const_iterator i = children.begin(); i != children.end(); i++ ) {
        (*i)->cloneDAG( newNodes );
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
const VectorString& ContainerNode::getDAGClass( void ) const {

    static VectorString rbClass = VectorString( ContainerNode_name ) + DeterministicNode::getDAGClass();
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
VectorNatural ContainerNode::getIndex( const DAGNode* element ) const {

    if ( container == NULL )
        throw RbException( element->getName() + " is not an element of " + getName() );
    
    return container->getIndex( element );
}


/** Is it possible to mutate to type and dim? */
bool ContainerNode::isMutableTo( const std::string& valType, int dim ) const {

    // First check whether our slots and children allow it
    ContainerNode* dummy = new ContainerNode( valType, dim );
    bool           ok    = DeterministicNode::isMutableTo( dummy );
    if ( !ok )
        return false;

    // Refuse to change dimensionality
    if ( dim != getDim() )
        return false;

    // Now check that all parents are OK with it
    if ( container == NULL ) {

        ValueContainer* valContainer = static_cast<ValueContainer*>( value );
        for ( size_t i = 0; i < valContainer->size(); i++ ) {
        
            const RbObject * theElem = valContainer->elements[i];
            if ( theElem != NULL && !theElem->isConvertibleTo( valType, false ) )
                return false;
        }
    }
    else {
        for ( size_t i = 0; i < container->size(); i++ ) {
        
            const RbObject * theElem = (*container)[i]->getValue();
            if ( theElem != NULL && !theElem->isConvertibleTo( valType, false ) )
                return false;
        }
    }

    return true;
}


/** Is it possible to change parent node oldNode to newNode? */
bool ContainerNode::isParentMutableTo( const DAGNode* oldNode, const DAGNode* newNode ) const {

    // Check that old node is among parents
    if ( parents.find( const_cast<DAGNode*>( oldNode ) ) == parents.end() )
        throw RbException( "Node is not an element" );

    // Check that new node is of value type
    if ( Workspace::userWorkspace().isXOfTypeY( newNode->getValueType(), valueType ) )
        return true;

    return false;
}


/** Mutate to type and dim */
ContainerNode* ContainerNode::mutateTo( const std::string& valType, int dim ) const {

    // This call will convert us to a variable container, if we are a simple value container
    if ( !isMutableTo( valType, dim ) )
        throw RbException( "Invalid container type conversion" );

    // Refuse to modify dimensionality if not caught by isMutableTo
    assert( dim == getDim() );

    // Now create and fill the mutant
    VariableContainer* temp = new VariableContainer( TypeSpec( valType, dim ), container->getLength() );
    ContainerNode*     copy = new ContainerNode( temp );
    for ( size_t i = 0; i < container->size(); i++ ) {
    
        const RbObject * theElem = (*container)[i]->getValue();
        if ( theElem != NULL ) {
            if ( theElem->isType( valType ) )
                (*copy->container)[i] = (*container)[i];
            else
                (*copy->container)[i] = new ConverterNode( (*container)[i], valType, 0 );
        }
    }

    return copy;
}


/** Print struct for user */
void ContainerNode::printStruct( std::ostream& o ) const {

    o << "_DAGClass    = " << getDAGClass() << std::endl;
    o << "_valueType   = " << valueType << std::endl;
    o << "_dim         = " << getDim() << std::endl;
    o << "_length      = " << VectorInteger( container->getLength() ) << std::endl;
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


/** Get complete info about object */
std::string ContainerNode::richInfo(void) const {

    std::ostringstream o;

    o << "Container node:" << std::endl;
    o << "dim         = " << getDim() << std::endl; 
    o << "length      = " << VectorInteger(container->getLength()) << std::endl;
    o << "valueType   = " << valueType << std::endl;
    o << "container   = " << std::endl;
    if ( container == NULL )
        o << "container   = NULL" << std::endl;
    else {
        o << "container   = " << std::endl;
        o << *container << std::endl;
    }
    o << "value       = " << std::endl;
    o << value << std::endl;

    if ( storedValue == NULL )
        o << "storedValue = NULL" << std::endl;
    else {
        o << "storedValue = " << std::endl;
        o << storedValue << std::endl;
    }

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
void ContainerNode::update( void ) {

    if ( touched && !changed ) {
        
        assert( storedValue == NULL );

        storedValue = value;
        value = container->getConstValue();
        changed = true;
    }
}

