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
    MemberFunction(retType, argRules) {

}


/** Clone the object */
SimpleMemberFunction* SimpleMemberFunction::clone(void) const {

    return new SimpleMemberFunction(*this);
}


RlValue<RbLanguageObject> SimpleMemberFunction::execute( void ) {
    return RbFunction::execute();
}


/** Execute function: call the object's internal implementation through executeOperation */
RbPtr<RbLanguageObject> SimpleMemberFunction::executeFunction( const std::vector<const RbObject *> &args ) {

    return object->executeSimpleMethod( funcName, args );

}


/** Get class name of object */
const std::string& SimpleMemberFunction::getClassName(void) { 
    
    static std::string rbClassName = "Simple Member function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& SimpleMemberFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( MemberFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}



