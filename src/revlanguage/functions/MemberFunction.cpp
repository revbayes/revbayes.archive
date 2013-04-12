/**
 * @file
 * This file contains the implementation of SimpleMemberFunction, which is used
 * to map member function calls (member method calls) of complex objects
 * to internal functions instead of providing regular Function objects
 * implementing the member functions.
 *
 * @brief Implementation of SimpleMemberFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-15 18:59:11 +0200 (Tue, 15 May 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: SimpleMemberFunction.cpp 1544 2012-05-15 16:59:11Z hoehna $
 */

#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "MemberFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Constructor */
MemberFunction::MemberFunction(const TypeSpec retType, ArgumentRules* argRules) : 
Function(), argumentRules(argRules), object(NULL), returnType(retType) {
    
}


/** Clone the object */
MemberFunction* MemberFunction::clone(void) const {
    
    return new MemberFunction(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
RbLanguageObject* MemberFunction::execute( void ) {
    
    return object->executeMethod( funcName, args );
    
}


/** Get class name of object */
const std::string& MemberFunction::getClassName(void) { 
    
    static std::string rbClassName = "Member function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& MemberFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& MemberFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& MemberFunction::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get return type */
const TypeSpec& MemberFunction::getReturnType(void) const {
    
    return returnType;
}


void MemberFunction::setMemberObject( RbLanguageObject *obj) {
    
    // we do not own the object itself because one object can have multiple member functions
    object = obj;
}



void MemberFunction::setMethodName(std::string const &name) {
    funcName = name;
}


/** We catch here the setting of the argument variables to store our parameters. */
void MemberFunction::setArgumentVariable(std::string const &name, const RbPtr<const Variable> &var) {
    
    
    // We expect a couple of parameters which we need to add to the member function. Therefore we do not call the base class.
    //        Function::setArgumentVariable(name, var);
}

