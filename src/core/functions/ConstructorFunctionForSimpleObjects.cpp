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
#include "Vector.h"
#include "VectorString.h"

#include <sstream>


// Definition of the static type spec member
const TypeSpec ConstructorFunctionForSimpleObjects::typeSpec(ConstructorFunctionForSimpleObjects_name);

/** Constructor */
ConstructorFunctionForSimpleObjects::ConstructorFunctionForSimpleObjects(RbPtr<RbLanguageObject> obj) : RbFunction(), templateObject(obj) {
    
    argRules = templateObject->getMemberRules();
}


/** Clone the object */
ConstructorFunctionForSimpleObjects* ConstructorFunctionForSimpleObjects::clone(void) const {
    
    return new ConstructorFunctionForSimpleObjects(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbLanguageObject* ConstructorFunctionForSimpleObjects::executeFunction(void) {
    
    RbLanguageObject* copy = templateObject->clone();
    
    Vector* params = new Vector(RbObject_name);
    for ( size_t i = 0; i < args->size(); i++ ) {
        params->push_back( (*args)[i]->getValue() );
//        copy->setMemberVariable( (*args)[i]->getLabel(), (*args)[i]->getVariable() );
    }
    
    copy->initialize(params);
    
    return copy;
}


/** Get argument rules */
const ArgumentRules* ConstructorFunctionForSimpleObjects::getArgumentRules(void) const {
    
    return ( argRules );
}


/** Get class vector describing type of object */
const VectorString& ConstructorFunctionForSimpleObjects::getClass(void) const { 
    
    static VectorString rbClass = VectorString(ConstructorFunctionForSimpleObjects_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const TypeSpec& ConstructorFunctionForSimpleObjects::getReturnType(void) const {
    
    return templateObject->getTypeSpec();
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& ConstructorFunctionForSimpleObjects::getTypeSpec(void) const {
    return typeSpec;
}

