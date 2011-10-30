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
DagNodeContainer::DagNodeContainer(void) : Container(RbLanguageObject_name), typeSpec(DagNodeContainer_name) {
    
}

/** Set type of elements */
DagNodeContainer::DagNodeContainer(size_t l) : Container(RbLanguageObject_name), typeSpec(DagNodeContainer_name) {
    resize(l-1);
}


/* Destructor */
DagNodeContainer::~DagNodeContainer(void) {
    // call clear which frees the memory of the elements
    clear();
}


/** Clear contents of value container */
void DagNodeContainer::clear( void ) {
    
    elements.clear();
}

/** clone */
DagNodeContainer* DagNodeContainer::clone(void) const {
    
    return new DagNodeContainer(*this);
}

RbObject* DagNodeContainer::convertTo(TypeSpec const &type) const {
    
    if (type.getBaseType() == Vector_name) {
        // test whether each object in the container is actually a constant node holding a value
        Vector* valueVector = new Vector(*type.getElementType());
        for (std::vector<RbPtr<VariableSlot> >::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            RbPtr<DAGNode> theNode = (*it)->getDagNodePtr();
            if (theNode->isType(ConstantNode_name) && theNode->getValue()->isTypeSpec(*type.getElementType())) {
                RbPtr<RbObject> element( theNode->getValue().get() );
                valueVector->push_back(element);
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

/** Get class DagNodeContainer describing type of object */
const VectorString& DagNodeContainer::getClass(void) const { 
    
    static VectorString rbClass = VectorString(DagNodeContainer_name) + ConstantMemberObject::getClass();
	return rbClass;
}

/** Get element */
RbPtr<const RbObject> DagNodeContainer::getElement(const size_t index) const {
    
    // test if the index is outside the current range
    if (index >= size()) {
        // Yes, it is outside the range and we through a error
        throw RbException("Index out of bounds in DagNodeContainer.");
    }
    
    return RbPtr<const RbObject>(elements[index].get());
}


/** Get element */
RbPtr<RbObject> DagNodeContainer::getElement(const size_t index) {
    
    // test if the index is outside the current range
    if (index >= size()) {
        // Yes, it is outside the range and we through a error
        throw RbException("Index out of bounds in DagNodeContainer.");
    }
    
    return RbPtr<RbObject>(elements[index].get());
}


/** Get the type spec of this class. We return a member variable because instance might have different types of elements. */
const TypeSpec& DagNodeContainer::getTypeSpec(void) const {
    return typeSpec;
}


/** Can we convert this DAG node container into another object? */
bool DagNodeContainer::isConvertibleTo(TypeSpec const &type) const {
    
    if (type.getBaseType() == Vector_name) {
        // test whether each object in the container is actually a constant node holding a value
        for (std::vector<RbPtr<VariableSlot> >::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            RbPtr<DAGNode> theNode = (*it)->getDagNodePtr();
            if (!theNode->isType(ConstantNode_name) || !theNode->getValue()->isTypeSpec(*type.getElementType())) {
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


void DagNodeContainer::push_back(RbPtr<RbObject> x) {
    
    // we expect to receive a parameter of type Variable
    RbPtr<Variable> var( dynamic_cast<Variable*>(x.get()) );
    if (var != NULL) {
        RbPtr<VariableSlot> theSlot( new VariableSlot(EmptyString,var) );
        elements.push_back(theSlot);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}


void DagNodeContainer::push_front(RbPtr<RbObject> x) {
    
    // we expect to receive a parameter of type Variable
    RbPtr<Variable> var( dynamic_cast<Variable*>(x.get()) );
    if (var != NULL) {
        RbPtr<VariableSlot> theSlot( new VariableSlot(EmptyString,var) );
        elements.insert( elements.begin(), theSlot );
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}




/** Print value for user */
void DagNodeContainer::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<RbPtr<VariableSlot> >::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
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
        RbPtr<Variable> emptyVar( new Variable(EmptyString) );
        RbPtr<VariableSlot> emptySlot( new VariableSlot( EmptyString, emptyVar ) );
        elements.push_back( emptySlot );
    }
    
}


/** Complete info about object */
std::string DagNodeContainer::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "DAG Node Container: value = ";
    printValue(o);
    
    return o.str();
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, RbPtr<RbObject> elem) {
    if (index >= size()) {
        throw RbException("Cannot set element in DagNodeContainer outside the current range.");
    }
    // we expect to receive a parameter of type Variable
    RbPtr<Variable> var( dynamic_cast<Variable*>(elem.get()) );
    if (var.get() != NULL) {
        RbPtr<VariableSlot> theSlot = elements[index];
        theSlot->setVariable(var);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, RbPtr<Variable> elem) {
    if (index >= size()) {
        throw RbException("Cannot set element in DagNodeContainer outside the current range.");
    }
    RbPtr<VariableSlot> theSlot = elements[index];
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
    std::vector<RbPtr<VariableSlot> > uniqueVector;
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




