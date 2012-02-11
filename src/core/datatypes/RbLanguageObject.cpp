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


/** Convert to type and dim. The caller manages the returned object. */
RbObject* RbLanguageObject::convertTo(const TypeSpec& type) const {
    
    
    return RbObject::convertTo(type);
}

/** Encode the object into a serializable xml-object */
XmlElement* RbLanguageObject::encode(XmlDocument* doc, const std::string& name) {    
    
    //! @Sebatian TODO basic implementation which should return an empty xml object with only the name
    
    return NULL;
}


/** Get class vector describing type of object */
const VectorString& RbLanguageObject::getClass(void) const { 
    
    static VectorString rbClass = VectorString(RbLanguageObject_name) + RbObject::getClass();
	return rbClass; 
}



/** Return member rules (no members) */
const MemberRules& RbLanguageObject::getMemberRules(void) const {
    
    static const MemberRules rules = MemberRules();
    
    return rules;
}


/** 
 * Initialize this instance with the parameters given. 
 * The ordering of the parameters is specified by the member rules. 
 */
void RbLanguageObject::initialize(const Vector& params) {
    // do nothing because we do not have any parameters. Some derived classes might have ...
}


/** Is convertible to type? */
bool RbLanguageObject::isConvertibleTo(const TypeSpec& type) const {
    
    return RbObject::isConvertibleTo(type);
}


/** Make sure we can print the value of the object easily */
std::ostream& operator<<(std::ostream& o, const RbLanguageObject& x) {
    
    x.printValue(o);
    return o;
}

