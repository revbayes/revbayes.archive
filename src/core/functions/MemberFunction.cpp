/**
 * @file
 * This file contains the implementation of MemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the member functions. Note that the first argument passed
 * in a member function call is a pointer to the MemberNode of the calling
 * object (like a this pointer).
 *
 * @brief Implementation of MemberFunction
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
#include "MemberFunction.h"
#include "MemberObject.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec MemberFunction::typeSpec(MemberFunction_name);

/** Constructor */
MemberFunction::MemberFunction(const TypeSpec retType, ArgumentRules* argRules) : 
    RbFunction(), argumentRules(argRules), object(NULL), returnType(retType) {

}

MemberFunction::~MemberFunction() {
    // we do not own the object
//    if (object != NULL) {
//        delete object;
//    }
}


/** Clone the object */
MemberFunction* MemberFunction::clone(void) const {

    return new MemberFunction(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
const RbLanguageObject& MemberFunction::execute( void ) {

    return object->executeOperation( funcName, *args );

}


/** Get class vector describing type of object */
const VectorString& MemberFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(MemberFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get argument rules */
const ArgumentRules& MemberFunction::getArgumentRules(void) const {

    return *argumentRules;
}


/** Get return type */
const TypeSpec& MemberFunction::getReturnType(void) const {

    return returnType;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& MemberFunction::getTypeSpec(void) const {
    return typeSpec;
}


void MemberFunction::setMemberObject(MemberObject& obj) {
    
    // we do not own the object itself because one object can have multiple member functions
//    if (object != NULL) {
//        delete object;
//    }
    object = &obj;
}

