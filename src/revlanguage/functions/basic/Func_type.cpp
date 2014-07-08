/**
 * @file
 * This file contains the implementation of Func_type, which is
 * the function used to get the type of a variable.
 *
 * @brief Implementation of Func_quit
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-05-04 18:03:37 +0200 (Fri, 04 May 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2012-09-07
 *
 * $Id: Func_source.cpp 1485 2012-05-04 16:03:37Z hoehna $
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_type.h"
#include "RbException.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_type::Func_type( void ) : Function() {
    
}


/** Clone object */
Func_type* Func_type::clone( void ) const {
    
    return new Func_type( *this );
}


/** Execute function */
RevPtr<Variable> Func_type::execute( void ) {
    
    RlString* type = new RlString( args[0].getVariable()->getRevObject().getType() );
    
    return new Variable( type );
}


/** Get argument rules */
const ArgumentRules& Func_type::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, RevObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_type::getClassName(void) { 
    
    static std::string rbClassName = "Func_type";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_type::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_type::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_type::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlString::getClassTypeSpec();
    
    return returnTypeSpec;
}

