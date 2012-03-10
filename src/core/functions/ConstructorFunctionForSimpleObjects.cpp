/**
 * @file
 * This file contains the implementation of ConstructorFunctionForSimpleObjects, which is used
 * for functions that construct simple objects.
 *
 * @brief Implementation of ConstructorFunctionForSimpleObjects
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
#include "ConstructorFunctionForSimpleObjects.h"
#include "DAGNode.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "RbVector.h"

#include <sstream>

/** Constructor */
ConstructorFunctionForSimpleObjects::ConstructorFunctionForSimpleObjects(RbLanguageObject* obj) : RbFunction(), templateObject(obj) {
    // Hack: We know that we do not own the member rules.
    argRules = &templateObject->getMemberRules();
}


/** Constructor */
ConstructorFunctionForSimpleObjects::ConstructorFunctionForSimpleObjects(const ConstructorFunctionForSimpleObjects& obj) : RbFunction(obj) {
    templateObject = obj.templateObject->clone();
    
    // Hack: We know that we do not own the member rules.
    argRules = &templateObject->getMemberRules();
}


ConstructorFunctionForSimpleObjects::~ConstructorFunctionForSimpleObjects() {
    delete templateObject;
}


/** Clone the object */
ConstructorFunctionForSimpleObjects* ConstructorFunctionForSimpleObjects::clone(void) const {
    
    return new ConstructorFunctionForSimpleObjects(*this);
}


/** Execute function: we reset our template object here and give out a copy */
const RbLanguageObject& ConstructorFunctionForSimpleObjects::executeFunction(void) {
    
    copyObject = templateObject->clone();
    
    std::vector<RbObject*> params;
    for ( size_t i = 0; i < args.size(); i++ ) {
        params.push_back( args[i].getVariable().getValue().clone() );
//        copy->setMemberVariable( (*args)[i]->getLabel(), (*args)[i]->getVariable() );
    }
    
    copyObject->initialize(params);
    
    return *copyObject;
}


/** Get argument rules */
const ArgumentRules& ConstructorFunctionForSimpleObjects::getArgumentRules(void) const {
    
    return *argRules;
}


/** Get class name of object */
const std::string& ConstructorFunctionForSimpleObjects::getClassName(void) { 
    
    static std::string rbClassName = "Constructor function for simple objects";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& ConstructorFunctionForSimpleObjects::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& ConstructorFunctionForSimpleObjects::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& ConstructorFunctionForSimpleObjects::getReturnType(void) const {
    
    return templateObject->getTypeSpec();
}



/** We catch here the setting of the argument variables to store our parameters. */
void ConstructorFunctionForSimpleObjects::setArgumentVariable(std::string const &name, const Variable* var) {
    
    // We do nothing here because the only thing we need is to collect the arguments in a vector.
    // This is done by the base class alreay.
    
}

