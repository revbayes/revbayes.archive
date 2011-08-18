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
#include "MonitorsContainer.h"
#include "MovesContainer.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "ConstantNode.h"


/** Set type of elements */
DagNodeContainer::DagNodeContainer(void) : Container(TypeSpec(DAGNode_name)) {
    
}

/** Set type of elements */
DagNodeContainer::DagNodeContainer(size_t l) : Container(TypeSpec(DAGNode_name)) {
    resize(l);
    
    // set the length
    length = l;
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
    
    length = 0;
}

/** clone */
DagNodeContainer* DagNodeContainer::clone(void) const {
    
    return new DagNodeContainer(*this);
}

RbLanguageObject* DagNodeContainer::convertTo(std::string const &type) const {
    
    // test for type conversion
    if (type == MovesContainer_name) {
        
        // test whether each object in the container is actually a constant node holding a move
        MovesContainer* moves = new MovesContainer();
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
    else if (type == MonitorsContainer_name) {
        
        // test whether each object in the container is actually a constant node holding a move
        MonitorsContainer* monitors = new MonitorsContainer();
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
    
    static VectorString rbClass = VectorString(DagNodeContainer_name) + Container::getClass();
	return rbClass;
}

/** Get element */
VariableSlot* DagNodeContainer::getElement(const size_t index) {
    
    // test if the index is outside the current range
    if (index >= length) {
        // Yes, it is outside the range and we need resizing
        resize(index + 1);
    }
    
    return elements[index];
}


/** Can we convert this DAG node container into another object? */
bool DagNodeContainer::isConvertibleTo(std::string const &type, bool once) const {
    
    // test for type conversion
    if (type == MovesContainer_name) {
        
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
    else if (type == MonitorsContainer_name) {
        
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
    
    // reset the length
    length = elements.size();
}


/** Complete info about object */
std::string DagNodeContainer::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "DAG Node Container: value = ";
    printValue(o);
    
    return o.str();
}


/** Set element */
void DagNodeContainer::setElement(const size_t index, Variable *elem) {
    if (index >= length) {
        throw RbException("Cannot set element in DagNodeContainer outside the current range.");
    }
    VariableSlot *theSlot = elements[index];
    theSlot->setVariable(elem);
}

