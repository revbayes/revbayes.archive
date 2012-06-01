/**
 * @file
 * This file contains the implementation of Func_reference, which is
 * the function used to create reference of another variable.
 *
 * @brief Implementation of Func_reference
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

#include "ArgumentRule.h"
#include "Func_reference.h"
#include "RbException.h"
#include "RbUtil.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <fstream>


/** Clone object */
Func_reference* Func_reference::clone( void ) const {
    
    return new Func_reference( *this );
}


/** Execute function */
RbPtr<RbLanguageObject> Func_reference::executeFunction( const std::vector<const RbObject*>& args ) {
    
    // reference to the original variable
    const RbLanguageObject* val = static_cast<const RbLanguageObject*>( args[0] ); 
    
    return RbPtr<RbLanguageObject>( val->clone() );
}


/** Get argument rules */
const ArgumentRules& Func_reference::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "variable", true, RbLanguageObject::getClassTypeSpec() ) );
        rulesSet = true;
    }

    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_reference::getClassName(void) { 
    
    static std::string rbClassName = "Reference function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_reference::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_reference::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_reference::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbLanguageObject::getClassTypeSpec();
    return returnTypeSpec;
}

