/**
 * @file
 * This file contains the implementation of Func_clear, which is
 * the function used to list the content of the workspace.
 *
 * @brief Implementation of Func_clear
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "Func_clear.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"


/** Clone object */
Func_clear* Func_clear::clone( void ) const {
    
    return new Func_clear( *this );
}


/** Execute function */
const RbLanguageObject& Func_clear::executeFunction( void ) {
    
    // clear the user workspace
    Workspace::userWorkspace().clear();
    
    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_clear::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_clear::getClassName(void) { 
    
    static std::string rbClassName = "Clear function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_clear::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_clear::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_clear::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

