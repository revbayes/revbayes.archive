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

#include "RbException.h"
#include "RbNames.h"
#include "RbObject.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>


/** Brief info about object: use static class attribute */
std::string RbObject::briefInfo(void) const {

    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}


/** Convert to object of language type typeSpec. We simply pass this
    through to the function that uses just the type and the dim.     */
RbObject* RbObject::convertTo(const TypeSpec& typeSpec) const {

    return convertTo( typeSpec.getType(), typeSpec.getDim() );
}


/** Convert to type and dim. The caller manages the returned object. */
RbObject* RbObject::convertTo(const std::string& type, int dim) const {

    std::ostringstream  msg;
    msg << "Type conversion to " << TypeSpec(type, dim) << " not supported";
    throw RbException( msg );
}

/** encode the object into a serializable xml-object */
const XmlElement* RbObject::encode(XmlDocument *doc, const std::string& name) {    
    // dummy function
    
    //TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& RbObject::getClass(void) const { 

    static VectorString rbClass = VectorString(RbObject_name);
	return rbClass; 
}


/** Get object type (first entry in class vector) */
const std::string& RbObject::getType(void) const {

    return getClass()[0];
}


/** Get language type spec for object */
const TypeSpec RbObject::getTypeSpec(void) const {

    return TypeSpec( getClass()[0], getDim() );
}


/** Is convertible to type and dim? */
bool RbObject::isConvertibleTo(const TypeSpec& typeSpec, bool once) const {

    return isConvertibleTo( typeSpec.getType(), typeSpec.getDim(), once );
}


/** Is convertible to type and dim? */
bool RbObject::isConvertibleTo(const std::string& type, int dim, bool once) const {

    return false;
}


/** Are we of specified object type? We need to check entire class vector in case we are derived from type. */
bool RbObject::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/**
 * Are we of specified language type? In the general case we need
 * to resort to the workspace because we could be a container type
 * without information about the class vector of our elements, which
 * could be absent or NULL at the time.
 */
bool RbObject::isTypeSpec(const TypeSpec& typeSpec) const {

    return Workspace::userWorkspace().isXOfTypeY( getTypeSpec(), typeSpec );
}


/** Print info about object simply by using richInfo function */
void RbObject::print(std::ostream& o) const {

    o << richInfo() << std::endl;
}

/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbObject& x) {

    x.printValue(o);
    return o;
}

