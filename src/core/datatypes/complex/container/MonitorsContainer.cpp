/**
 * @file
 * This file contains the declaration of MonitorsContainer, a complex type
 * that acts as a base class for all MonitorsContainers.
 *
 * @brief Declaration of MonitorsContainer
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

#include "MonitorsContainer.h"
#include "RbException.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"


/** Set type of elements */
MonitorsContainer::MonitorsContainer(void) : Container(TypeSpec(Monitor_name)) {
    
}

/** Set type of elements */
MonitorsContainer::MonitorsContainer(size_t l) : Container(TypeSpec(Monitor_name)) {
    resize(l);
    
    // set the length
    length = l;
}


/* Destructor */
MonitorsContainer::~MonitorsContainer(void) {
    clear();
}


/** Clear contents of value container and make length 0 in all dimensions */
void MonitorsContainer::clear( void ) {
    
    for ( std::vector<Monitor*>::iterator i = elements.begin(); i != elements.end(); i++ ) {
        (*i)->release();
        if ((*i)->isUnreferenced()) {
            delete ( *i );
        }
    }
    elements.clear();
    
    length = 0;
}

/** clone */
MonitorsContainer* MonitorsContainer::clone(void) const {
    
    return new MonitorsContainer(*this);
}


/** Get class MonitorsContainer describing type of object */
const VectorString& MonitorsContainer::getClass(void) const { 
    
    static VectorString rbClass = VectorString(MonitorsContainer_name) + Container::getClass();
	return rbClass;
}

/** Get element */
Monitor* MonitorsContainer::getElement(const size_t index) {
    
    // test if the index is outside the current range
    if (index >= length) {
        // Yes, it is outside the range and we need resizing
        resize(index + 1);
    }
    
    return elements[index];
}



/** Add a new move object */
void MonitorsContainer::push_back(Monitor *m) {
    m->retain();
    elements.push_back(m);
    
    length++;
}


/** Print value for user */
void MonitorsContainer::printValue( std::ostream& o ) const {
    
    o << "[ ";
    for ( std::vector<Monitor*>::const_iterator i = elements.begin(); i != elements.end(); i++ ) {
        if ( i != elements.begin() )
            o << ", ";
        if ( (*i) == NULL )
            o << "NULL";
        else
            (*i)->printValue(o);
    }
    o <<  " ]";
    
}


/** Resize MonitorsContainer */
void MonitorsContainer::resize( size_t n ) {
    
    // we do not allow to shrink the container
    if ( n < elements.size() )
        throw RbException( "Invalid attempt to shrink MonitorsContainer" );
    
    // add NULL elements for each new element
    for ( size_t i = elements.size(); i < n; i++ ) {
        elements.push_back( NULL );
    }
    
    // reset the length
    length = elements.size();
}


/** Complete info about object */
std::string MonitorsContainer::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "Monitor Container: value = ";
    printValue(o);
    
    return o.str();
}


/** Set element */
void MonitorsContainer::setElement(const size_t index, Monitor *elem) {
    if (index >= length) {
        throw RbException("Cannot set element in MonitorsContainer outside the current range.");
    }
    Monitor *oldMonitor = elements[index];
    
    // release the old monitor
    if (oldMonitor != NULL) {
        oldMonitor->release();
        if (oldMonitor->isUnreferenced()) {
            delete oldMonitor;
        }
    }
    
    elements.erase(elements.begin() + index);
    
    // set the new monitor
    elements.insert(elements.begin() + index, elem);
    
    // retain the new element
    elem->retain();
}

