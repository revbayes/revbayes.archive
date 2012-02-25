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

/** Copy Constructor: we just delegate */
MutableMemberObject::MutableMemberObject(const MutableMemberObject& m) : MemberObject(m) {
    memberVariables = m.memberVariables;
}


/** Get class name of object */
const std::string& MutableMemberObject::getClassName(void) { 
    
    static std::string rbClassName = "Mutable member object";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MutableMemberObject::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberObject::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Is the object Mutable? */
bool MutableMemberObject::isConstant( void ) const {
    
    return false;
}


/** Print value for user */
void MutableMemberObject::printValue(std::ostream& o) const {
    
    for ( size_t i = 0; i < members.size(); i++ ) {
        
        o << "." << memberVariables[i].getLabel() << std::endl;
        memberVariables[i].getValue().printValue(o);
        o << std::endl << std::endl;
    }
}



