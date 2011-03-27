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
ContainerNode::~ContainerNode(void) {

    if (numRefs() != 0)
        throw RbException ("Cannot delete container node with references"); 

    /* Remove parents first */
    for (std::set<DAGNode*>::iterator i=parents.begin(); i!=parents.end(); i++)
        (*i)->removeChildNode(this);
    parents.clear();

    delete container;    // This will delete any DAG nodes that need to be deleted

    delete value;
    if (storedValue)
        delete storedValue;
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


/** Get class vector describing type of DAG node */
const VectorString& ContainerNode::getDAGClass(void) const {

    static VectorString rbClass = VectorString(ContainerNode_name) + DeterministicNode::getDAGClass();
    return rbClass;
}


#if 0
/** Get value element */
const RbObject* ContainerNode::getValElement( const VectorInteger& index ) const {

    return container->getElement( index );
}


/** Get variable element */
const DAGNode* ContainerNode::getVarElement( const VectorInteger& index ) const {

    return container[ index ];
}
#endif


/** Is it possible to mutate to newNode? */
bool ContainerNode::isMutableTo( const DAGNode* newNode ) const {

    return false;
}


/** Is it possible to mutate node to language type typeSpec? */
bool ContainerNode::isMutableTo( const TypeSpec& typeSpec ) const {

    bool isMutable = false;

    return isMutable;
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


/* Mutate to new language type typeSpec */
ContainerNode* ContainerNode::mutateTo( const TypeSpec& typeSpec ) {

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


#if 0
/** Set value element or elements from value */
void ContainerNode::setElement(const VectorInteger& index, RbObject* val) {

    // Resize if necessary
    if (index.size() == length.size()) {
        VectorInteger tempLen  = length;
        bool      growSize = false;
        for (size_t i=0; i<index.size(); i++) {
            if (index[i] >= tempLen[i]) {
                tempLen[i] = index[i] + 1;
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    size_t offset = getOffset(index);

    // Find target node
    std::vector<DAGNode*>::iterator targetIt = nodes.begin() + offset;

    // Check if parser wants to set multiple elements
    if (index.size() < length.size()) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw(RbException("Invalid assignment: Container to subcontainer"));

        // Check that the source is a container
        RbComplex* source = dynamic_cast<RbComplex*>(val);
        if (source == NULL || source->getDim() == 0)
            throw RbException("Source does not have elements");
    
        // Count number of elements
        const VectorInteger& sourceLen = source->getLength();
        int numSourceElements = 1;
        for (size_t i=0; i<sourceLen.size(); i++)
            numSourceElements *= sourceLen[i];
        int numTargetElements = 1;
        for (size_t i=index.size(); i<length.size(); i++)
            numTargetElements *= length[i];

        // Throw an error if a mismatch
        if (numSourceElements != numTargetElements)
            throw RbException("Unequal source and target sizes");

        // Cycle through assignments
        for (ContainerIterator i=source->begin(); i!=source->end(); ++i) {

            // Do the assignment
            const RbObject* elem = source->getElement(i);
            if (elem->isType(getValueType())) {
                if ((*targetIt) != NULL && (*targetIt)->isDAGType(StochasticNode_name))
                    ((StochasticNode*)(*targetIt))->setValue(elem->clone());
                else {
                    if ((*targetIt) != NULL) {
                        (*targetIt)->removeChildNode(this);
                        if ((*targetIt)->numRefs() == 0)
                            delete (*targetIt);
                        parents.erase(*targetIt);
                    }
                    (*targetIt) = new ConstantNode(elem->clone());
                    (*targetIt)->addChildNode(this);
                    parents.insert(*targetIt);
                }
            }
            else
                throw (RbException("Incompatible types"));

            // Increase target iterator
            targetIt++;
        }
        delete val;     // We are responsible for deleting the useless container
        return;
    }

    // Parser wants to set a single element
    if (val->isType(getValueType())) {
        if ((*targetIt) != NULL && (*targetIt)->isDAGType(StochasticNode_name)) {
            // We just set the value of the stochastic node
            ((StochasticNode*)(*targetIt))->setValue(val);
        }
        else {
            // We replace the node with a constant node
            if ((*targetIt) != NULL) {
                (*targetIt)->removeChildNode(this);
                if ((*targetIt)->numRefs() == 0)
                    delete (*targetIt);
                parents.erase(*targetIt);
            }
            (*targetIt) = new ConstantNode(val);
            (*targetIt)->addChildNode(this);
            parents.insert(*targetIt);
        }
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    changed = false;
    touchAffected();
}


/** Set element from DAG node */
void ContainerNode::setElement(const VectorInteger& index, DAGNode* var) {

    // Resize if necessary
    if (index.size() == length.size()) {
        VectorInteger tempLen  = length;
        bool      growSize = false;
        for (size_t i=0; i<index.size(); i++) {
            if (index[i] >= tempLen[i]) {
                tempLen[i] = index[i] + 1;
                growSize = true;
            }
            else if (index[i] < 0)
                throw (RbException("Nonpositive index"));
        }
        if (growSize)
            resize(tempLen);
    }

    // Get offset; also checks for errors in index
    size_t offset = getOffset(index);

    // Get target iterator
    std::vector<DAGNode*>::iterator targetIt = nodes.begin() + offset;

    // Check if parser wants to set multiple elements
    if (index.size() < length.size()) {

        /* TODO: Do we want to allow this? See code below if we do */
        throw(RbException("Invalid assignment"));

        // Check that the source is a plate
        if (var->getDim() == 0)
            throw (RbException("Source does not have elements"));

        ContainerNode* source = dynamic_cast<ContainerNode*>(var);
    
        // Count number of elements
        const VectorInteger& sourceLen = source->getLength();
        int numSourceElements = 1;
        for (size_t i=0; i<sourceLen.size(); i++)
            numSourceElements *= sourceLen[i];
        int numTargetElements = 1;
        for (size_t i=index.size(); i<length.size(); i++)
            numTargetElements *= length[i];

        // Throw an error if a mismatch
        if (numSourceElements != numTargetElements)
            throw (RbException("Unequal source and target sizes"));

        // Cycle through assignments
        for (ContainerIterator i=source->begin(); i!=source->end(); ++i) {

            // Do the assignment
            const DAGNode* elem = (*source)[i];
            if (Workspace::userWorkspace().isXOfTypeY(elem->getValueType(), valueType)) {
                if ((*targetIt) != NULL) {
                    (*targetIt)->removeChildNode(this);
                    if ((*targetIt)->numRefs() == 0)
                        delete (*targetIt);
                    parents.erase(*targetIt);
                }
                (*targetIt) = elem->clone();
                (*targetIt)->addChildNode(this);
                parents.insert(*targetIt);
            }
            else
                throw (RbException("Incompatible types"));

            // Increase target iterator
            targetIt++;
        }
        if (var->numRefs() == 0)
            delete var;     // We are responsible for deleting the useless plate
        return;
    }

    // Parser wants to set a single element
    if (var == NULL || Workspace::userWorkspace().isXOfTypeY(var->getValueType(), valueType)) {
        if ((*targetIt) != NULL) {
            // If conversion of constant to stochastic node, save value
            if ((*targetIt)->isDAGType(ConstantNode_name) &&
                var->isDAGType(StochasticNode_name) &&
                (*targetIt)->getValue() != NULL &&
                (*targetIt)->getValue()->isType(var->getValueType())) {
                ((StochasticNode *)var)->setValue((*targetIt)->getValue()->clone());
            }
            (*targetIt)->removeChildNode(this);
            if ((*targetIt)->numRefs() == 0)
                delete (*targetIt);
            parents.erase(*targetIt);
        }
        (*targetIt) = NULL;
        if (var != NULL) {
            if (var->getName() != "")
                throw RbException("Cannot use reference in setting element of DAG node plate");
            std::ostringstream varName;
            varName << getName();
            for (size_t i=0; i<index.size(); i++)
                varName << "[" << index[i]+1 << "]";
            var->addChildNode(this);
            parents.insert(var);
        }
        (*targetIt) = var;
    }
    else
        throw (RbException("Incompatible types"));

    touched = true;
    changed = false;
    touchAffected();
}


/** Set value of plate */
void ContainerNode::setValue(RbObject* val) {

    throw RbException("Invalid assignment of value to node plate");
}

#endif


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

