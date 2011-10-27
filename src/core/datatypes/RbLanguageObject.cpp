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
RbObject* RbLanguageObject::convertTo(const TypeSpec& type) const {
    
    
    return RbObject::convertTo(type);
}

/** Encode the object into a serializable xml-object */
const RbPtr<XmlElement> RbLanguageObject::encode(RbPtr<XmlDocument> doc, const std::string& name) {    
    
    //! @Sebatian TODO basic implementation which should return an empty xml object with only the name
    
    return RbPtr<XmlElement>::getNullPtr();
}


/** Get class vector describing type of object */
const VectorString& RbLanguageObject::getClass(void) const { 
    
    static VectorString rbClass = VectorString(RbLanguageObject_name) + RbObject::getClass();
	return rbClass; 
}


/** Is convertible to type and dim? */
bool RbLanguageObject::isConvertibleTo(const TypeSpec& type) const {
    
    return RbObject::isConvertibleTo(type);
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

