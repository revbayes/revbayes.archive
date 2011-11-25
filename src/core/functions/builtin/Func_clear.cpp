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
#include "RbUtil.h"
#include "TypeSpec.h"
#include "VectorString.h"
#include "Workspace.h"

#include <fstream>


// Definition of the static type spec member
const TypeSpec Func_clear::typeSpec(Func_clear_name);
const TypeSpec Func_clear::returnTypeSpec(RbVoid_name);

/** Clone object */
Func_clear* Func_clear::clone( void ) const {
    
    return new Func_clear( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_clear::executeFunction( void ) {
    
    // clear the user workspace
    Workspace::userWorkspace()->clear();
    
    return RbPtr<RbLanguageObject>::getNullPtr();
}


/** Get argument rules */
RbPtr<const ArgumentRules> Func_clear::getArgumentRules( void ) const {
    
    static RbPtr<ArgumentRules> argumentRules( new ArgumentRules() );
    
    return RbPtr<const ArgumentRules>( argumentRules );
}


/** Get class vector describing type of object */
const VectorString& Func_clear::getClass( void ) const {
    
    static VectorString rbClass = VectorString( Func_clear_name ) + RbFunction::getClass();
    return rbClass;
}


/** Get return type */
const TypeSpec& Func_clear::getReturnType( void ) const {
    
    return returnTypeSpec;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Func_clear::getTypeSpec(void) const {
    return typeSpec;
}

