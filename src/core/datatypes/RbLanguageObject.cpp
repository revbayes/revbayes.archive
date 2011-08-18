/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for language objects, RbLanguageObject.
 *
 * @brief Partial implementation of RbLanguageObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbLanguageObject
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "RbException.h"
#include "RbUtil.h"
#include "RbLanguageObject.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <sstream>

/** Brief info about object: use static class attribute */
std::string RbLanguageObject::briefInfo(void) const {
    
    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}

/** Convert to type and dim. The caller manages the returned object. */
RbLanguageObject* RbLanguageObject::convertTo(const std::string& type) const {
    
    return NULL;
}

/** Encode the object into a serializable xml-object */
const XmlElement* RbLanguageObject::encode(XmlDocument *doc, const std::string& name) {    
    
    //! @Sebatian TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& RbLanguageObject::getClass(void) const { 
    
    static VectorString rbClass = VectorString(RbLanguageObject_name) + RbObject::getClass();
	return rbClass; 
}


/** Is convertible to type and dim? */
bool RbLanguageObject::isConvertibleTo(const std::string& type, bool once) const {
    
    return false;
}


/** Are we of specified object type? We need to check entire class vector in case we are derived from type.
 *  We provide a special case for Scalar_name to avoid having to introduce an extra class for primitive
 *  types. The function is overridden in RbComplex, so that complex objects are not of scalar type.
 */
bool RbLanguageObject::isType(const std::string& type) const {
    
    const VectorString& classVec = getClass();
    
    for (size_t i=0; i<classVec.getLength(); i++) {
        if (type == classVec[i])
            return true;
    }
    
	return false;
}


/** Print info about object simply by using richInfo function */
void RbLanguageObject::print(std::ostream& o) const {
    
    o << richInfo() << std::endl;
}


/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbLanguageObject& x) {
    
    x.printValue(o);
    return o;
}

