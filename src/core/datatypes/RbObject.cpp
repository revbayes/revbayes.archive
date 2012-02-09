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
#include "RbUtil.h"
#include "RbObject.h"
#include "TypeSpec.h"
#include "Vector.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cassert>
#include <sstream>


// Definition of the static type spec member
const TypeSpec RbObject::typeSpec(RbObject_name);


RbObject::RbObject(void) {
    
}

/** Destructor */
RbObject::~RbObject() {

}

/** 
 * Brief info about object: 
 * Return the brief debug information about this object.
 * This is the default implementation which you can overwrite. It return only the class name.
 */
std::string RbObject::briefInfo(void) const {
    
    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}

/** Convert to object of language type typeSpec with base type and element type.
    Here we just convert from scalar types to vectors and overwritten function do more fancy stuff. */
RbObject* RbObject::convertTo(const TypeSpec& typeSpec) const {

    if (typeSpec.getBaseType() == Vector_name) {
        Vector *v = new Vector(typeSpec.getElementType()->getBaseType());
        v->push_back( this->clone() );
        
        return v;
    }
    
    return NULL;
}


/** Encode the object into a serializable xml-object */
XmlElement* RbObject::encode(XmlDocument* doc, const std::string& name) {    
    
    //! @Sebatian TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& RbObject::getClass(void) const { 

    static VectorString rbClass = VectorString(RbObject_name);
	return rbClass; 
}

/** Get type spec */
const TypeSpec& RbObject::getTypeSpec( void ) const {
    
    return typeSpec;
}



/** Get element or subcontainer at index. This is a convenience function implemented here so that we don't have to cast to container. 
 If this object is not a container, we throw an excpetion */
const RbObject& RbObject::getElement(size_t index) const {
    
    std::ostringstream  msg;
    msg << "Illegal call to getElement in type \"" << getType() << "\"";
    throw RbException( msg );
}


/** Get element or subcontainer at index. This is a convenience function implemented here so that we don't have to cast to container. 
 If this object is not a container, we throw an excpetion */
RbObject& RbObject::getElement(size_t index) {
    
    std::ostringstream  msg;
    msg << "Illegal call to getElement in type \"" << getType() << "\"";
    throw RbException( msg );
}


/** Get object type */
const std::string& RbObject::getType(void) const {

    return getTypeSpec().toString();
}


/** Is convertible to type and dim? */
bool RbObject::isConvertibleTo(const TypeSpec& typeSpec) const {
    
    if (typeSpec.getBaseType() == Vector_name && isTypeSpec(*typeSpec.getElementType())) {
        return true;
    }

    return false;
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbObject::isType(const std::string& type) const {
    
    const VectorString& classVec = getClass();
    
    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }
    
	return false;
}


/** Are we of specified language type? */
bool RbObject::isTypeSpec(const TypeSpec& typeSpec) const {
    
    const VectorString& classVec = getClass();
    const std::string&  type     = typeSpec.getType();
    
    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

    return false;
}


/** 
 * Rich info about object: 
 * The richInfo gives detailed debug information.
 * This is the default implementation when no detailed information is available and therefore it calls briefInfo.
 */
std::string RbObject::richInfo(void) const {
    
    return briefInfo();
}


/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbObject& x) {
    
    x.printValue(o);
    return o;
}



