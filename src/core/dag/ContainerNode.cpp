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
 * $Id:$
 */


#include "Boolean.h"
#include "ConstantNode.h"
#include "Container.h"
#include "ContainerNode.h"
#include "ConverterNode.h"
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbNames.h"
#include "RbString.h"
#include "StochasticNode.h"
#include "ValueContainer.h"
#include "VariableContainer.h"
#include "VectorIndex.h"
#include "VectorInteger.h"
#include "VectorNatural.h"
#include "VectorString.h"
#include "Workspace.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>


/** Construct empty container node */
ContainerNode::ContainerNode( const std::string& valType, size_t dim )
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
    copy->container = new VariableContainer( container->getTypeSpec(), container->getLength() );
    for ( size_t i = 0; i < container->size(); i++ ) {

        DAGNode* theParentClone = (*container)[i]->cloneDAG(newNodes);
        delete (*copy->container)[i];
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


/** Convert candidate value element(s) */
RbObject* ContainerNode::convertValueElement( const VectorInteger& index, RbObject* valueElem ) const {

    if ( index.size() > size_t( getDim() ) ) {
        delete valueElem;
        throw RbException( "Too many indices when setting element of " + getName() + index.toIndexString() );
    }

    size_t emptyDim = getDim();
    for ( size_t i=0; i<index.size(); i++ ) {
        if ( index[i] >= 0 )
            emptyDim--;
    }

    if ( valueElem->isTypeSpec( TypeSpec( valueType, emptyDim ) ) )
        return valueElem;

    if ( valueElem->isConvertibleTo( valueType, emptyDim, true ) ) {
        RbObject* temp = valueElem->convertTo( valueType, emptyDim );
        delete valueElem;
        return temp;
    }

    delete valueElem;
    throw RbException( "Type mismatch when setting " + getName() + index.toIndexString() );
}


/** Convert candidate variable element(s) */
DAGNode* ContainerNode::convertVariableElement( const VectorInteger& index, DAGNode* variableElem, bool convert ) const {

    if ( index.size() > size_t( getDim() ) ) {
        delete variableElem;
        throw RbException( "Too many indices when setting element of " + getName() + index.toIndexString() );
    }

    size_t emptyDim = getDim();
    for ( size_t i=0; i<index.size(); i++ ) {
        if ( index[i] >= 0 )
            emptyDim--;
    }

    if ( Workspace::userWorkspace().isXOfTypeY( variableElem->getTypeSpec(), TypeSpec( valueType, emptyDim ) ) )
        return variableElem;

    if ( convert == true && Workspace::userWorkspace().isXConvertibleToY( variableElem->getTypeSpec(), TypeSpec( valueType, emptyDim ) ) ) {

        RbObject* temp = variableElem->getValue()->convertTo( valueType, emptyDim );
        delete variableElem;
        
        return temp->wrapIntoVariable();
    }
    
    if ( variableElem->numRefs() == 0 )
        delete variableElem;
    throw RbException( "Type mismatch when setting " + getName() + index.toIndexString() );
}


/** Get class vector describing type of DAG node */
const VectorString& ContainerNode::getDAGClass( void ) const {

    static VectorString rbClass = VectorString( ContainerNode_name ) + DeterministicNode::getDAGClass();
    return rbClass;
}


/** Convenient vector access */
DAGNode* ContainerNode::getElement( size_t i ) {

    if ( container != NULL )
        return (*container)[i];
    else
        throw RbException( "No direct element access to value container from ContainerNode" );
}


/** Get element for parser. This version supports empty indices, giving you subcontainers back. */
DAGNode* ContainerNode::getElement( VectorIndex& index ) const {

    if ( container != NULL )
        return container->getElement( index );
    else
        return static_cast<ValueContainer*>( value )->getElement( index );
}


/** Get (or make) single element for parser */
DAGNode* ContainerNode::getElement( const VectorNatural& index ) const {
    
    // Check for erroneous index
    if ( index.size() != getDim() )
        throw RbException( "Dimension mismatch in index to container element" );
    
    if ( container != NULL )
        return container->getElement( index );
    else
        return static_cast<ValueContainer*>( value )->getElement( index );
}


/** Get element owner for parser */
DAGNode* ContainerNode::getElementOwner( VectorIndex& index ) {

    if ( index.size() > getDim() )
        throw RbException( getName() + index.toIndexString() + " does not exist" );

    return this;
}


/** Get index of specified element */
VectorNatural ContainerNode::getIndex( const DAGNode* element ) const {

    if ( container == NULL )
        throw RbException( element->getName() + " is not an element of " + getName() );
    
    return container->getIndex( element );
}


/** Is node a constant value? */
bool ContainerNode::isConst( void ) const {

    if ( container == NULL )
        return true;
    else
        return container->isConstant();
}


/** Is it possible to mutate to type and dim? */
bool ContainerNode::isMutableTo( const std::string& valType, size_t dim ) const {

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
            if ( theElem != NULL && !theElem->isConvertibleTo( valType, true ) )
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


/** Check if a candidate value element is valid */
bool ContainerNode::isValidElement( const VectorInteger& index, const RbObject* val ) const {

    size_t emptyDim = getDim();
    for ( size_t i=0; i<index.size(); i++ ) {
        if ( index[i] >= 0 )
            emptyDim--;
    }

    if ( val == NULL && val->getDim() == emptyDim )
        return true;

    if ( val->isTypeSpec( TypeSpec( valueType, emptyDim ) ) )
        return true;

    if ( val->isConvertibleTo( valueType, emptyDim, true ) )
        return true;

    return false;
}


/** Check if a candidate variable element is valid */
bool ContainerNode::isValidElement( const VectorInteger& index, DAGNode* variable, bool convert ) const {

    if ( variable->isImmutable() )
        return isValidElement( index, variable->getValue() );

    size_t emptyDim = getDim();
    for ( size_t i=0; i<index.size(); i++ ) {
        if ( index[i] >= 0 )
            emptyDim--;
    }

    if ( Workspace::userWorkspace().isXOfTypeY( variable->getTypeSpec(), TypeSpec( valueType, emptyDim ) ) )
        return true;

    if ( convert == true && Workspace::userWorkspace().isXConvertibleToY( variable->getTypeSpec(), TypeSpec( valueType, emptyDim ) ) )
        return true;

    return false;
}


/** Mutate to type and dim */
ContainerNode* ContainerNode::mutateTo( const std::string& valType, size_t dim ) const {

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


/** Set variable element(s); including assignment to single elements and to subcontainers */
void ContainerNode::setElement( VectorIndex& index, DAGNode* var, bool convert ) {

    // Catch empty index
    if ( index.size() == 0 ) {

        if ( var->numRefs() == 0 )
            delete var;
        throw RbException( "Empty index when assigning to element of " + getName() );
    }

    // Catch superfluous indices
    if ( index.size() > size_t( getDim() ) ) {

        if ( var->numRefs() == 0 )
            delete var;
        throw RbException( "Too many indices when assigning to " + getName() );
    }

    // Translate to integer index
    VectorInteger intIndex;
    if ( container != NULL )
        intIndex = container->getIntegerIndex( index );
    else
        intIndex = static_cast<ValueContainer*>( value )->getIntegerIndex( index );

    // Calculate empty dim
    size_t emptyDim = getDim();
    for ( size_t i=0; i<intIndex.size(); i++ ) {
        if ( index[i] >= 0 )
            emptyDim--;
    }

    // Convert element container or element; throws an error if conversion not possible
    var = convertVariableElement( intIndex, var, convert );

    // Transform ourselves into a variable container if need be
    if ( container == NULL && ( convert == false || var->isImmutable() == false ) ) {
    
        container = new VariableContainer( static_cast<ValueContainer&>( *value ) );
    }

    // Check indices and see if we need to resize
    bool grow = false;
    std::vector<size_t> containerLen = static_cast<ValueContainer*>( value )->getLength();
    std::vector<size_t> newLen       = containerLen;
    for ( size_t i = 0; i < intIndex.size(); i++ ) {

        if ( intIndex[i] >= int( containerLen[i] ) ) {
            newLen[i] = intIndex[i] + 1;
            grow = true;
        }
    }

    // Set the element with iteration if necessary
    touched = true;
    touchAffected();
    changed = false;
    if ( emptyDim == 0 ) {

        // Get element index
        VectorNatural elemIndex;
        for ( size_t i=0; i<intIndex.size(); i++ )
            elemIndex.push_back( intIndex[i] );
        
        // Set single element; grow if necessary
        if ( container != NULL ) {

            // Grow if necessary
            if ( grow )
                container->resize( newLen );

            // Delete the old element
            size_t offset = container->getOffset( intIndex );
            container->elements[offset]->removeChildNode( this );
            if ( container->elements[offset]->numRefs() == 0 )
                delete container->elements[offset];
            parents.erase( container->elements[offset] );

            // Set the new element
            container->elements[offset] = var;
            var->addChildNode( this );
            parents.insert( var );
        }
        else {
            ValueContainer* valContainer = static_cast<ValueContainer*>( value );
            if ( grow )
                valContainer->resize( newLen );
            static_cast<ValueContainer*>( value )->setElement( elemIndex, var->getValue()->clone() );
            if ( var->numRefs() == 0 )
                delete var;
        }
    }
    else {
        
        // Get handy pointer to source elements
        assert( var->isDAGType( ContainerNode_name ) );
        ContainerNode* elemVar = static_cast<ContainerNode*>( var );

        // Use iteration to do subcontainer assignment
        if ( container != NULL ) {

            // Grow if necessary
            if ( grow )
                container->resize( newLen );

            ContainerIterator it2 = static_cast<const ValueContainer*>( elemVar->getValue() )->begin();
            for ( ContainerIterator it1 = container->begin(); it1 != container->end(); it1++ ) {
            
                size_t i;
                for ( i=0; i<intIndex.size(); i++ ) {
                    if ( intIndex[i] >= 0 && it1[i] != intIndex[i] )
                        break;
                }

                if ( i == intIndex.size() ) {

                    // Delete the old element
                    size_t offset = container->getOffset( it1 );
                    container->elements[offset]->removeChildNode( this );
                    if ( container->elements[offset]->numRefs() == 0 )
                        delete container->elements[offset];
                    parents.erase( container->elements[offset] );

                    // Set the new element
                    VectorNatural vecNat(it2++);
                    DAGNode* elem = elemVar->getElement( vecNat );
                    container->elements[offset] = elem;
                    elem->addChildNode( this );
                    parents.insert( elem );
                }
            }
        }
        else {

            ValueContainer* containerVar = static_cast<ValueContainer*>( value );
            if ( grow )
                containerVar->resize( newLen );
            ContainerIterator it2 = static_cast<const ValueContainer*>( elemVar->getValue() )->begin();
            for ( ContainerIterator it1 = containerVar->begin(); it1 != containerVar->end(); it1++ ) {
            
                size_t i;
                for ( i=0; i<intIndex.size(); i++ ) {
                    if ( index[i] >= 0 && it1[i] != intIndex[i] )
                        break;
                }

                if ( i == index.size() ) {
                    VectorNatural vecNat( it2++ );
                    containerVar->setElement( it1, elemVar->getElement( vecNat )->getValue()->clone() );
                }
            }
        }
    }
}


/** Set container element (simple version, no subcontainer assignment) */
void ContainerNode::setElement( VectorNatural& index, DAGNode* var, bool convert ) {

    if ( convert )
        var = convertVariableElement(index, var );

    if ( container != NULL )

        container->setElement( index, var );

    else {
        
        static_cast<ValueContainer*>( value )->setElement( index, var->getValue()->clone() );

        if ( var->numRefs() == 0 )
            delete var;
    }
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

