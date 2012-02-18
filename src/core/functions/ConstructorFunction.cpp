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
#include "ConstructorFunction.h"
#include "DAGNode.h"
#include "MemberObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec ConstructorFunction::typeSpec(ConstructorFunction_name);

/** Constructor */
ConstructorFunction::ConstructorFunction(MemberObject* obj) : RbFunction(), templateObject(obj), copyObject( NULL ) {

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

ConstructorFunction::~ConstructorFunction() {
    delete templateObject;
    delete copyObject;
    // we do not delete the argRules because we know that we do not own them!
}


ConstructorFunction& ConstructorFunction::operator=(const ConstructorFunction &c) {
    
    if (this != &c) {
        RbFunction::operator=(c);
        
        delete templateObject;
        delete copyObject;
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
const RbLanguageObject& ConstructorFunction::executeFunction(void) {

    if (copyObject != NULL) {
        delete copyObject;
    }
    copyObject = templateObject->clone();

    for ( size_t i = 0; i < args->size(); i++ ) {
        copyObject->setMember( (*args)[i].getLabel(), (*args)[i].getVariable().getDagNode() );
    }
 
    return *copyObject;
}


/** Get argument rules */
const ArgumentRules& ConstructorFunction::getArgumentRules(void) const {

    return *argRules;
}


/** Get class vector describing type of object */
const VectorString& ConstructorFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(ConstructorFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const TypeSpec& ConstructorFunction::getReturnType(void) const {

    return templateObject->getTypeSpec();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ConstructorFunction::getTypeSpec(void) const {
    return typeSpec;
}

