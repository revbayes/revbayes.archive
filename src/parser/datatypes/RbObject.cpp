/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RbObject.
 *
 * @brief Partial implementation of RbObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbObject
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "RbException.h"
#include "RbObject.h"
#include "RbUtil.h"
#include "RbVector.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <cassert>
#include <sstream>


RbObject::RbObject(void) {
    refCount = 0;
}


RbObject::RbObject(const RbObject &o) {
    refCount = 0;
}

/** Destructor */
RbObject::~RbObject() {

}


RbObject& RbObject::operator=(const RbObject &o) {
    // check for self assignment
    if ( this == &o ) {
        // nothing actually to do
    }
    
    return *this;
}


/** Convert to object of language type typeSpec with base type and element type.
    Here we just convert from scalar types to vectors and overwritten function do more fancy stuff. */
RbObject* RbObject::convertTo(const TypeSpec& typeSpec) const {

    // test if we need to convert this object into an vector
    if ( typeSpec.getBaseType() == RbVector::getClassName() ) {
        RbVector *v = new RbVector( typeSpec.getElementType() );
        v->push_back( this->clone() );
        
        // we rely on the implemented conversion functions inside the vector class
        RbObject* returnVector = v->convertTo( typeSpec );
        
        // we need to free the temporary vector object
        delete v;
        
        return returnVector;
    }
    
    throw RbException("Failed conversion from " + getTypeSpec() + " to " + typeSpec);
    
    return NULL;
}


/* Decrement the reference count. */
size_t RbObject::decrementReferenceCount( void ) const {
    // this is OK!
    const_cast<RbObject*>( this )->refCount--;
    
    return refCount;
}


/** 
 * Rich info about object: 
 * The debugInfo gives detailed debug information.
 * This is the default implementation when no detailed information is available 
 * and therefore it prints the class name and the value.
 */
std::string RbObject::debugInfo(void) const {
    
    std::ostringstream o;
    o << "Object of type: ";
    o << getClassTypeSpec();
    o << std::endl;
    o << "Value: ";
    printValue(o);
    return o.str();
}



/** Encode the object into a serializable xml-object */
XmlElement* RbObject::encode(XmlDocument* doc, const std::string& name) {    
    
    //! @Sebatian \TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class name of object */
const std::string& RbObject::getClassName(void) { 
    
    static std::string rbClassName = "Object";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& RbObject::getClassTypeSpec(void) { 

    static TypeSpec rbClass = TypeSpec( getClassName() );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RbObject::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/* Get the reference count for this instance. */
size_t RbObject::getReferenceCount(void) const {
    return refCount;
}


/* Increment the reference count for this instance. */
void RbObject::incrementReferenceCount( void ) const {
    const_cast<RbObject*>( this )->refCount++;
}


/* Is convertible to type? */
bool RbObject::isConvertibleTo(const TypeSpec& typeSpec) const {
    
    if (typeSpec.getBaseType() == RbVector::getClassName() && isTypeSpec(typeSpec.getElementType())) {
        return true;
    }

    return false;
}


/* Are we of specified language type? */
bool RbObject::isTypeSpec(const TypeSpec& typeSpec) const {
    
    return getTypeSpec().isDerivedOf( typeSpec );
}


/* @TODO: Temporary default implementation. (Sebastian: Needs removal and should be pure virtual!) */
void RbObject::setLeanValue(const RbValue<void *> &val) {
    
    RbValue<void *> oldVal;
    oldVal.value = getLeanValue( oldVal.lengths );
    if ( val.value == oldVal.value ) {
        std::cerr << "Assignment to same memory address.\n";
    }
    else {
        std::cerr << "Assignment to new memory address.\n";
    }
}



/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbObject& x) {
    
    x.printValue(o);
    return o;
}



