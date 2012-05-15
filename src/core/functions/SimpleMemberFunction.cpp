/**
 * @file
 * This file contains the implementation of SimpleMemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the member functions. Note that the first argument passed
 * in a member function call is a pointer to the MemberNode of the calling
 * object (like a this pointer).
 *
 * @brief Implementation of SimpleMemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Ellipsis.h"
#include "SimpleMemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
SimpleMemberFunction::SimpleMemberFunction(const TypeSpec retType, ArgumentRules* argRules) : 
    RbFunction(), argumentRules(argRules), object(NULL), returnType(retType) {

}


/** Clone the object */
SimpleMemberFunction* SimpleMemberFunction::clone(void) const {

    return new SimpleMemberFunction(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
RbPtr<RbLanguageObject> SimpleMemberFunction::executeFunction( const std::vector<const RbObject *> &args ) {

    return object->executeSimpleMethod( funcName, args );

}


/** Get class name of object */
const std::string& SimpleMemberFunction::getClassName(void) { 
    
    static std::string rbClassName = "Member function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SimpleMemberFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& SimpleMemberFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& SimpleMemberFunction::getArgumentRules(void) const {

    return *argumentRules;
}


/** Get return type */
const TypeSpec& SimpleMemberFunction::getReturnType(void) const {

    return returnType;
}


void SimpleMemberFunction::setMemberObject( const RbPtr<MemberObject>& obj) {
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
}


/** We catch here the setting of the argument variables to store our parameters. */
void SimpleMemberFunction::setArgumentVariable(std::string const &name, const RbPtr<const RbLanguageObject> &var) {
    
    
    // We expect a couple of parameters which we need to add to the member function. Therefore we do not call the base class.
    //        RbFunction::setArgumentVariable(name, var);
}

