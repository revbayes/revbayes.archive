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
#include "MemberNode.h"
#include "MemberObject.h"
#include "RbNames.h"
#include "TypeSpec.h"
#include "VectorString.h"

#include <sstream>


/** Constructor */
ConstructorFunction::ConstructorFunction(MemberObject* obj) 
    : RbFunction(), templateObject(obj) {

    argRules = templateObject->getMemberRules();
    for ( size_t i = 0; i < argRules.size(); i++ )
        argRules[i]->setReference( true );
}


/** Clone the object */
ConstructorFunction* ConstructorFunction::clone(void) const {

    return new ConstructorFunction(*this);
}


/** Execute function: we reset our template object here and give out a copy */
DAGNode* ConstructorFunction::execute(void) {

    const ArgumentRules& argRules = getArgumentRules();

    MemberObject* copy = templateObject->clone();

    ArgumentRules::const_iterator i;
    std::vector<VariableSlot>::const_iterator j;
    for ( size_t i = 0; i < args.size(); i++ )
        copy->setVariable( args.getLabel(i), args[i].getReference() );
 
    return new MemberNode(copy);
}


/** Get argument rules */
const ArgumentRules& ConstructorFunction::getArgumentRules(void) const {

    return argRules;
}


/** Get class vector describing type of object */
const VectorString& ConstructorFunction::getClass(void) const { 

    static VectorString rbClass = VectorString(ConstructorFunction_name) + RbFunction::getClass();
	return rbClass; 
}


/** Get return type */
const TypeSpec ConstructorFunction::getReturnType(void) const {

    return templateObject->getTypeSpec();
}

