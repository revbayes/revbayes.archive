/**
 * @file
 * This file contains the implementation of ConstructorFunction, which is used
 * for functions that construct member objects.
 *
 * @brief Implementation of ConstructorFunction
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
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "MemberObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
ConstructorFunction::ConstructorFunction(const RbPtr<MemberObject> &obj) : RbFunction(), templateObject(obj), copyObject( NULL ) {

    // Hack: we know that we will not own the argRules.
    argRules = &templateObject->getMemberRules();
}


/** Constructor */
ConstructorFunction::ConstructorFunction(const ConstructorFunction& obj) : RbFunction(obj), copyObject( NULL ) {
    
    templateObject = obj.templateObject->clone();
    if (obj.copyObject != NULL) {
        copyObject = obj.copyObject->clone();
    }
    
    // Hack: we know that we will not own the argRules.
    argRules = &templateObject->getMemberRules();
}


ConstructorFunction& ConstructorFunction::operator=(const ConstructorFunction &c) {
    
    if (this != &c) {
        RbFunction::operator=(c);
        
        templateObject = c.templateObject->clone();
        if (c.copyObject != NULL) {
            copyObject = c.copyObject->clone();
        }
        else {
            copyObject = NULL;
        }
        
        // Hack: we know that we will not own the argRules.
        argRules = &templateObject->getMemberRules();
    }
    
    return *this;
}


/** Clone the object */
ConstructorFunction* ConstructorFunction::clone(void) const {

    return new ConstructorFunction(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbPtr<RbLanguageObject> ConstructorFunction::executeFunction(const std::vector<const RbObject *> &args) {
    
    copyObject = templateObject->clone();

    for ( size_t i = 0; i < args.size(); i++ ) {
        copyObject->setConstMember( this->args[i]->getLabel(), RbPtr<const Variable>( new Variable( new ConstantNode( RlValue<RbLanguageObject>( RbPtr<RbLanguageObject>( static_cast<RbLanguageObject*>( args[i]->clone() ) ) ) ) ) ) );
    }
 
    return RbPtr<RbLanguageObject>( copyObject );
}


/** Get argument rules */
const ArgumentRules& ConstructorFunction::getArgumentRules(void) const {

    return *argRules;
}


/** Get class name of object */
const std::string& ConstructorFunction::getClassName(void) { 
    
    static std::string rbClassName = "Constructor function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstructorFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstructorFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& ConstructorFunction::getReturnType(void) const {

    return templateObject->getTypeSpec();
}



