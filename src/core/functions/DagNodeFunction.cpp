/**
 * @file
 * This file contains the implementation of DagNodeFunction, which is used
 * to map DagNode function calls (DagNode method calls) of complex objects
 * to internal functions instead of providing regular RbFunction objects
 * implementing the DagNode functions. Note that the first argument passed
 * in a DagNode function call is a pointer to the DagNodeNode of the calling
 * object (like a this pointer).
 *
 * @brief Implementation of DagNodeFunction
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-10 12:55:11 +0100 (Sat, 10 Mar 2012) $
 * @author The RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-17, version 1.0
 *
 * $Id: DagNodeFunction.cpp 1327 2012-03-10 11:55:11Z hoehna $
 */

#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "DAGNode.h"
#include "Ellipsis.h"
#include "DagNodeFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"

#include <sstream>


/** Constructor */
DagNodeFunction::DagNodeFunction(const TypeSpec retType, ArgumentRules* argRules) : 
RbFunction(), argumentRules(argRules), node(NULL), returnType(retType) {
    
}

DagNodeFunction::~DagNodeFunction() {
    // we do not own the object
    //    if (object != NULL) {
    //        delete object;
    //    }
}


/** Clone the object */
DagNodeFunction* DagNodeFunction::clone(void) const {
    
    return new DagNodeFunction(*this);
}


/** Execute function: call the object's internal implementation through executeOperation */
const RbLanguageObject& DagNodeFunction::execute( void ) {
    
    return node->executeOperation( funcName, args );
    
}


/** Get class name of object */
const std::string& DagNodeFunction::getClassName(void) { 
    
    static std::string rbClassName = "DagNode function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& DagNodeFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& DagNodeFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get argument rules */
const ArgumentRules& DagNodeFunction::getArgumentRules(void) const {
    
    return *argumentRules;
}


/** Get return type */
const TypeSpec& DagNodeFunction::getReturnType(void) const {
    
    return returnType;
}


void DagNodeFunction::setDagNode(DAGNode& obj) {
    
    // we do not own the object itself because one object can have multiple DagNode functions
    node = &obj;
}


/** We catch here the setting of the argument variables to store our parameters. */
void DagNodeFunction::setArgumentVariable(std::string const &name, const Variable* var) {
    
    
    // We expect a couple of parameters which we need to add to the DagNode function. Therefore we do not call the base class.
    //        RbFunction::setArgumentVariable(name, var);
}

