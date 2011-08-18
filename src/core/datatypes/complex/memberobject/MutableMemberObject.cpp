/**
 * @file
 * This file contains the partial implementation of MutableMemberObject, an abstract
 * base class for complex objects with member methods and variables.
 *
 * @brief Partial implementation of MutableMemberObject
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "Integer.h"
#include "MemberFunction.h"
#include "MutableMemberObject.h"
#include "RbException.h"
#include "RbFunction.h"
#include "RbUtil.h"
#include "VectorString.h"
#include "VariableNode.h"


/** Constructor: we set member variables here from member rules */
MutableMemberObject::MutableMemberObject() : MemberObject() {
    
}

/** Constructor: we set member variables here from member rules */
MutableMemberObject::MutableMemberObject(const MemberRules& memberRules) : MemberObject(memberRules) {
    
}


/** Convert to type: throw an error */
RbLanguageObject* MutableMemberObject::convertTo(const std::string& type) const {
    
    std::ostringstream msg;
    msg << "Type conversion of " << getTypeSpec() << " to " << TypeSpec(type) << " not supported";
    throw RbException(msg);
}

/** Get class vector describing type of object */
const VectorString& MutableMemberObject::getClass(void) const {
    
    static VectorString rbClass = VectorString(MutableMemberObject_name) + MemberObject::getClass();
    return rbClass;
}

/** Is the object Mutable? */
bool MutableMemberObject::isConstant( void ) const {
    
    return false;
}


/** Is convertible to type and dim? Default is false for member objects; override if you want to support type conversion */
bool MutableMemberObject::isConvertibleTo(const std::string& type, bool once) const {
    
	return false;
}


/** Print value for user */
void MutableMemberObject::printValue(std::ostream& o) const {
    
//    for ( size_t i = 0; i < members.size(); i++ ) {
//        
//        o << "." << members[i].getName() << std::endl;
//        if ( members[i].getValue() == NULL)
//            o << "NULL";
//        else
//            members[i].getValue()->printValue(o);
//        o << std::endl << std::endl;
//    }
}


/** Complete info about object */
std::string MutableMemberObject::richInfo(void) const {
    
    std::ostringstream o;
    o << getType() << ":" << std::endl;
    printValue(o);
    
    return o.str();
}


