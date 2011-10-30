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
ConstructorFunction::ConstructorFunction(RbPtr<MemberObject> obj) : RbFunction(), templateObject(obj) {

    argRules = templateObject->getMemberRules();
}


/** Clone the object */
ConstructorFunction* ConstructorFunction::clone(void) const {

    return new ConstructorFunction(*this);
}


/** Execute function: we reset our template object here and give out a copy */
RbPtr<RbLanguageObject> ConstructorFunction::execute(void) {

   RbPtr<MemberObject> copy( templateObject->clone() );

    for ( size_t i = 0; i < args->size(); i++ ) {
        copy->setMemberVariable( (*args)[i]->getLabel(), (*args)[i]->getVariable() );
    }
 
    return RbPtr<RbLanguageObject>( copy );
}


/** Get argument rules */
RbPtr<const ArgumentRules> ConstructorFunction::getArgumentRules(void) const {

    return ( argRules );
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

