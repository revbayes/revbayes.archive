/**
 * @file
 * This file contains the declaration of MovesContainer, a complex type
 * that acts as a base class for all MovesContainers.
 *
 * @brief Declaration of MovesContainer
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

#include "MovesContainer.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"


/** Set type of elements */
MovesContainer::MovesContainer(void) : Container(TypeSpec(Move_name)) {
    
}

/** Set type of elements */
MovesContainer::MovesContainer(size_t l) : Container(TypeSpec(Move_name)) {
    resize(l);
    
    // set the length
    length = l;
}


/* Destructor */
MovesContainer::~MovesContainer(void) {
    clear();
}


/** Clear contents of value container and make length 0 in all dimensions */
void MovesContainer::clear( void ) {
    
    for ( std::vector<Move*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
        (*i)->release();
        if ((*i)->isUnreferenced()) {
            delete ( *i );
        }
    }
    elements.clear();
    
    length = 0;
}

/** clone */
MovesContainer* MovesContainer::clone(void) const {
    
    return new MovesContainer(*this);
}


/** Get class MovesContainer describing type of object */
const VectorString& MovesContainer::getClass(void) const { 
    
    static VectorString rbClass = VectorString(MovesContainer_name) + Container::getClass();
	return rbClass;
}

/** Get element */
Move* MovesContainer::getElement(const size_t index) {
    
    // test if the index is outside the current range
    if (index >= length) {
        // Yes, it is outside the range and we need resizing
        resize(index + 1);
    }
    
    return elements[index];
}

/** Add a new move object */
void MovesContainer::push_back(Move *m) {
    m->retain();
    elements.push_back(m);
    
    length++;
}

/** Print value for user */
void MovesContainer::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<Move*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}


/** Resize MovesContainer */
void MovesContainer::resize( size_t n ) {
    
    // we do not allow to shrink the container
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink MovesContainer" );
    
    // add NULL elements for each new element
    for ( size_t i = elements.size(); i < n; i++ ) {
        elements.push_back( NULL );
    }
    
    // reset the length
    length = elements.size();
}


/** Complete info about object */
std::string MovesContainer::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "Move Container: value = ";
    printValue(o);
    
    return o.str();
}


/** Set element */
void MovesContainer::setElement(const size_t index, Move *elem) {
    if (index >= length) {
        throw RbException("Cannot set element in MovesContainer outside the current range.");
    }
    Move *oldMove = elements[index];
    
    // release the old Move
    if (oldMove != NULL) {
        oldMove->release();
        if (oldMove->isUnreferenced()) {
            delete oldMove;
        }
    }
    
    elements.erase(elements.begin() + index);
    
    // set the new Move
    elements.insert(elements.begin() + index, elem);
    
    // retain the new element
    elem->retain();
}

