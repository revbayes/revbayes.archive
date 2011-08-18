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
#include "RbNames.h"
#include "RbObject.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <cassert>
#include <sstream>

/** Destructor */
RbObject::~RbObject() {

//    assert(isUnreferenced());
//    if (!isUnreferenced())
//        throw RbException("Cannot delete referenced object.");
}



/** Convert to object of language type typeSpec. We simply pass this
    through to the function that uses just the type and the dim.     */
RbObject* RbObject::convertTo(const TypeSpec& typeSpec) const {

    return convertTo( typeSpec.getType() );
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* RbObject::convertTo(const std::string& type) const {

    std::ostringstream  msg;
    msg << "Type conversion to " << TypeSpec(type) << " not supported";
    throw RbException( msg );
}

/** Encode the object into a serializable xml-object */
const XmlElement* RbObject::encode(XmlDocument *doc, const std::string& name) {    
    
    //! @Sebatian TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& RbObject::getClass(void) const { 

    static VectorString rbClass = VectorString(RbObject_name);
	return rbClass; 
}

/** Get element or subcontainer at index. This is a convinience function implemented here so that we don't have to cast to container. 
 If this object is not a container, we throw an excpetion */
RbObject* RbObject::getElement(size_t index) {
    
    std::ostringstream  msg;
    msg << "Illegal call to getElement in type \"" << getType() << "\"";
    throw RbException( msg );
}


/** Get object type (first entry in class vector) */
const std::string& RbObject::getType(void) const {

    return getClass()[0];
}


/** Get language type spec for object */
const TypeSpec RbObject::getTypeSpec(void) const {

    return TypeSpec( getClass()[0] );
}


/** Is convertible to type and dim? */
bool RbObject::isConvertibleTo(const TypeSpec& typeSpec, bool once) const {

    return isConvertibleTo( typeSpec.getType(), once );
}


/** Is convertible to type and dim? */
bool RbObject::isConvertibleTo(const std::string& type, bool once) const {

    return false;
}


/** Are we of specified object type? We need to check entire class vector in case we are derived from type.
 *  We provide a special case for Scalar_name to avoid having to introduce an extra class for primitive
 *  types. The function is overridden in RbComplex, so that complex objects are not of scalar type.
 */
bool RbObject::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

    for (size_t i=0; i<classVec.getLength(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/** Are we of specified language type? This version works for
 *  all objects except for containers.
 */
bool RbObject::isTypeSpec(const TypeSpec& typeSpec) const {

    return isType( typeSpec.getType() );
}



