/**
 * @file
 * This file contains the declaration of DagNodeContainer, a complex type
 * that acts as a base class for all DagNodeContainers.
 *
 * @brief Declaration of DagNodeContainer
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-12-04, version 1.0
 * @extends Container
 *
 * $Id$
 */

#include "DagNodeContainer.h"
#include "Natural.h"
#include "Move.h"
#include "Monitor.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"
#include "ConstantNode.h"
#include <algorithm>

/** Set type of elements */
DagNodeContainer::DagNodeContainer(void) : Container( RbLanguageObject::getClassTypeSpec() ) {
    
}

/** Set type of elements */
DagNodeContainer::DagNodeContainer(size_t l) : Container( RbLanguageObject::getClassTypeSpec() ) {
    resize(l-1);
}

DagNodeContainer::DagNodeContainer(const DagNodeContainer& c) : Container(c) {
    
    for (std::vector<VariableSlot*>::const_iterator it = c.elements.begin(); it != c.elements.end(); it++) {
        elements.push_back((*it)->clone());
    }
}


/* Destructor */
DagNodeContainer::~DagNodeContainer(void) {
    // call clear which frees the memory of the elements
    clear();
}


DagNodeContainer& DagNodeContainer::operator=(const DagNodeContainer &c) {
    
    if (this != &c) {
        
        // free my memory
        clear();
        
        // copy the elements
        for (std::vector<VariableSlot*>::const_iterator it = c.elements.begin(); it != c.elements.end(); it++) {
            elements.push_back((*it)->clone());
        }
    }
    
    return *this;
}


/** Clear contents of value container */
void DagNodeContainer::clear( void ) {
    
    // free the memory of the single slots
    for (std::vector<VariableSlot*>::iterator it = elements.begin(); it != elements.end(); it++) {
        VariableSlot* theSlot = *it;
        delete theSlot;
    }
    
    elements.clear();
}

/** clone */
DagNodeContainer* DagNodeContainer::clone(void) const {
    
    return new DagNodeContainer(*this);
}

RbObject* DagNodeContainer::convertTo(TypeSpec const &type) const {
    
    if ( type.getBaseType() == Vector::getClassName() ) {
        // test whether each object in the container is actually a constant node holding a value
        Vector* valueVector = new Vector(type.getElementType());
        for (std::vector<VariableSlot* >::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode* theNode = (*it)->getDagNode();
            if (theNode->isTypeSpec( ConstantNode::getClassTypeSpec() ) && theNode->getValue().isTypeSpec(type.getElementType())) {
                const RbObject& element = theNode->getValue();
                valueVector->push_back(element.clone());
            }
            else {
                return NULL;
            }
        }
        
        // return the vector
        return valueVector;
    }
    
    
    return Container::convertTo(type);
}


/** Execute a member method. We overwrite the executeOperation function here because we return DAG nodes directly. */
const RbLanguageObject& DagNodeContainer::executeOperation(std::string const &name, const std::vector<Argument>& args) {
    
    return ConstantMemberObject::executeOperation( name, args );
}


/** Get class name of object */
const std::string& DagNodeContainer::getClassName(void) { 
    
    static std::string rbClassName = "DAG node container";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DagNodeContainer::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Container::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DagNodeContainer::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

/** Get element */
const RbObject& DagNodeContainer::getElement(const size_t index) const {
    
    // test if the index is outside the current range
    if (index >= size()) {
        // Yes, it is outside the range and we through a error
        throw RbException("Index out of bounds in DagNodeContainer.");
    }
    
    return *elements[index];
}


/** Get element */
RbObject& DagNodeContainer::getElement(const size_t index) {
    
    // test if the index is outside the current range
    if (index >= size()) {
        // Yes, it is outside the range and we through a error
        throw RbException("Index out of bounds in DagNodeContainer.");
    }
    
    return *elements[index];
}


/** Can we convert this DAG node container into another object? */
bool DagNodeContainer::isConvertibleTo(TypeSpec const &type) const {
    
    if ( type.getBaseType() == Vector::getClassName() ) {
        // test whether each object in the container is actually a constant node holding a value
        for (std::vector<VariableSlot* >::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode* theNode = (*it)->getDagNode();
            if (!theNode->isTypeSpec( ConstantNode::getClassTypeSpec() ) || !theNode->getValue().isTypeSpec(type.getElementType())) {
                return false;
            }
        }
        
        // return the true
        return true;
    }
    
    return Container::isConvertibleTo(type);
}


void DagNodeContainer::pop_back(void) {
    elements.pop_back();
}


void DagNodeContainer::pop_front(void) {
    elements.erase(elements.begin());
}


void DagNodeContainer::push_back(RbObject* x) {
    
    // we expect to receive a parameter of type Variable
    Variable* var = dynamic_cast<Variable*>( x );
    if (var != NULL) {
        VariableSlot* theSlot = new VariableSlot(EmptyString,var);
        elements.push_back(theSlot);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}


void DagNodeContainer::push_front(RbObject* x) {
    
    // we expect to receive a parameter of type Variable
    Variable* var = dynamic_cast<Variable*>( x );
    if (var != NULL) {
        VariableSlot* theSlot = new VariableSlot(EmptyString,var);
        elements.insert( elements.begin(), theSlot );
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}




/** Print value for user */
void DagNodeContainer::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<VariableSlot* >::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}


/** Resize DagNodeContainer */
void DagNodeContainer::resize( size_t n ) {
    
    // we do not allow to shrink the container
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink DagNodeContainer" );
    
    // add NULL elements for each new element
    for ( size_t i = elements.size(); i <= n; i++ ) {
        Variable* emptyVar = new Variable( RbObject::getClassTypeSpec() );
        VariableSlot* emptySlot = new VariableSlot( EmptyString, emptyVar );
        elements.push_back( emptySlot );
    }
    
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, RbObject* elem) {
    if (index >= size()) {
        std::ostringstream o;
        o << "Cannot set element at index " << index << " in DagNodeContainer(size=" << size()+1 << ") outside the current range.";
        throw RbException(o.str());
    }
    // we expect to receive a parameter of type Variable
    Variable* var = dynamic_cast<Variable*>( elem );
    if (var != NULL) {
        VariableSlot* theSlot = elements[index];
        theSlot->setVariable(var);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, Variable* elem) {
    if (index >= size()) {
        std::ostringstream o;
        o << "Cannot set element at index " << index << " in DagNodeContainer(size=" << size()+1 << ") outside the current range.";
        throw RbException(o.str());
    }
    VariableSlot* theSlot = elements[index];
    theSlot->setVariable(elem);
}


size_t DagNodeContainer::size(void) const {
    return elements.size();
}

/* Sort the vector */
void DagNodeContainer::sort( void ) {
    
    std::sort(elements.begin(), elements.end());
    return;
    
}

/* Remove duplicates and resize the vector */
void DagNodeContainer::unique(void) {
    
    sort();
    std::vector<VariableSlot* > uniqueVector;
    uniqueVector.push_back (elements[0]);
    for (size_t i = 1 ; i< elements.size() ; i++)
    {
        if (elements[i] != elements[i-1])
            uniqueVector.push_back(elements[i]);
    }
    
    clear();
    elements = uniqueVector;
    return;
    
}




