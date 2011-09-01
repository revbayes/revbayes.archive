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


/** Set type of elements */
DagNodeContainer::DagNodeContainer(void) : ConstantMemberObject() {
    
}

/** Set type of elements */
DagNodeContainer::DagNodeContainer(size_t l) : ConstantMemberObject() {
    resize(l);
}


/* Destructor */
DagNodeContainer::~DagNodeContainer(void) {
    // call clear which frees the memory of the elements
    clear();
}


/** Clear contents of value container and make length 0 in all dimensions */
void DagNodeContainer::clear( void ) {
    
    for ( std::vector<VariableSlot*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
        (*i)->release();
        if ((*i)->isUnreferenced()) {
            delete ( *i );
        }
    }
    elements.clear();
}

/** clone */
DagNodeContainer* DagNodeContainer::clone(void) const {
    
    return new DagNodeContainer(*this);
}

RbLanguageObject* DagNodeContainer::convertTo(std::string const &type) const {
    
    static const TypeSpec movesContainerType = TypeSpec(Vector_name,new TypeSpec(Move_name));
    static const TypeSpec monitorsContainerType = TypeSpec(Vector_name,new TypeSpec(Monitor_name));
    
    // test for type conversion
//    TypeSpec ts(type);
//    if (ts.getType() == Vector_name) {
//        // test whether each object in the container is actually a constant node holding a value
//        Vector* valueVector = new Vector(*ts.getElementType());
//        for (std::vector<VariableSlot*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
//            DAGNode *theNode = (*it)->getDagNodePtr();
//            if (theNode->isType(ConstantNode_name) && theNode->getValue()->isType(Move_name)) {
//                RbLanguageObject* element = theNode->getValuePtr();
//                valueVector->push_back(element);
//            }
//            else {
//                return NULL;
//            }
//        }
//        
//        // return the moves
//        return valueVector;
//    }
    
    
    
    if (type == movesContainerType.getType()) {
        
        // test whether each object in the container is actually a constant node holding a move
        Vector* moves = new Vector(TypeSpec(Move_name));
        for (std::vector<VariableSlot*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode *theNode = (*it)->getDagNodePtr();
            if (theNode->isType(ConstantNode_name) && theNode->getValue()->isType(Move_name)) {
                Move* theMove = dynamic_cast<Move*>(theNode->getValuePtr());
                moves->push_back(theMove);
            }
            else {
                return NULL;
            }
        }
        
        // return the moves
        return moves;
    }
    else if (type == monitorsContainerType.getType()) {
        
        // test whether each object in the container is actually a constant node holding a move
        Vector* monitors = new Vector(TypeSpec(Monitor_name));
        for (std::vector<VariableSlot*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode *theNode = (*it)->getDagNodePtr();
            if (theNode->isType(ConstantNode_name) && theNode->getValue()->isType(Monitor_name)) {
                Monitor* theMonitor = dynamic_cast<Monitor*>(theNode->getValuePtr());
                monitors->push_back(theMonitor);
            }
            else {
                return NULL;
            }
        }
        
        // return the monitors
        return monitors;
    }
    
    return NULL;
}

/** Get class DagNodeContainer describing type of object */
const VectorString& DagNodeContainer::getClass(void) const { 
    
    static VectorString rbClass = VectorString(DagNodeContainer_name) + ConstantMemberObject::getClass();
	return rbClass;
}

/** Get element */
VariableSlot* DagNodeContainer::getElement(const size_t index) const {
    
    // test if the index is outside the current range
    if (index >= size()) {
        // Yes, it is outside the range and we through a error
        throw RbException("Index out of bounds in DagNodeContainer.");
    }
    
    return elements[index];
}


/** Can we convert this DAG node container into another object? */
bool DagNodeContainer::isConvertibleTo(std::string const &type, bool once) const {
    
    
    static const TypeSpec movesContainerType = TypeSpec(Vector_name,new TypeSpec(Move_name));
    static const TypeSpec monitorsContainerType = TypeSpec(Vector_name,new TypeSpec(Monitor_name));
    
    // test for type conversion
    if (type == movesContainerType.getType()) {
        
        // test whether each object in the container is actually a constant node holding a move
        for (std::vector<VariableSlot*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode *theNode = (*it)->getDagNodePtr();
            if (!theNode->isType(ConstantNode_name) || !theNode->getValue()->isType(Move_name)) {
                return false;
            }
        }
        
        // return the moves
        return true;
    }
    else if (type == monitorsContainerType.getType()) {
        
        // test whether each object in the container is actually a constant node holding a monitor
        for (std::vector<VariableSlot*>::const_iterator it=elements.begin(); it!=elements.end(); it++) {
            DAGNode *theNode = (*it)->getDagNodePtr();
            if (!theNode->isType(ConstantNode_name) || !theNode->getValue()->isType(Monitor_name)) {
                return false;
            }
        }
        
        // return the monitors
        return true;
    }
    
    return false;
}


void DagNodeContainer::pop_back(void) {
    elements.pop_back();
}


void DagNodeContainer::pop_front(void) {
    elements.erase(elements.begin());
}


void DagNodeContainer::push_back(RbObject *x) {
    
    // we expect to receive a parameter of type Variable
    Variable *var           = dynamic_cast<Variable*>(x);
    if (var != NULL) {
        VariableSlot *theSlot   = new VariableSlot(EmptyString,var);
        elements.push_back(theSlot);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}


void DagNodeContainer::push_front(RbObject *x) {
    
    // we expect to receive a parameter of type Variable
    Variable *var           = dynamic_cast<Variable*>(x);
    if (var != NULL) {
        VariableSlot *theSlot   = new VariableSlot(EmptyString,var);
        elements.insert( elements.begin(), theSlot );
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
    
}




/** Print value for user */
void DagNodeContainer::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<VariableSlot*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
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
    for ( size_t i = elements.size(); i < n; i++ ) {
        Variable *emptyVar = new Variable(EmptyString);
        VariableSlot *emptySlot = new VariableSlot(EmptyString, emptyVar );
        emptySlot->retain();
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
void DagNodeContainer::setElement(const size_t index, RbObject *elem) {
    if (index >= size()) {
        throw RbException("Cannot set element in DagNodeContainer outside the current range.");
    }
    // we expect to receive a parameter of type Variable
    Variable *var           = dynamic_cast<Variable*>(elem);
    if (var != NULL) {
        VariableSlot *theSlot = elements[index];
        theSlot->setVariable(var);
    }
    else {
        throw RbException("Cannot add variable of non Variable type to DagNodeContainer.");
    }
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, Variable *elem) {
    if (index >= size()) {
        throw RbException("Cannot set element in DagNodeContainer outside the current range.");
    }
    VariableSlot *theSlot = elements[index];
    theSlot->setVariable(elem);
}


size_t DagNodeContainer::size(void) const {
    return elements.size();
}

