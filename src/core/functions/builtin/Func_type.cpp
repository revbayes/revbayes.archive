/**
 * @file
 * This file contains the implementation of Func_type, which is used
 * to print info about the type of a variable.
 *
 * @brief Implementation of Func_type
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_type.h"
#include "Integer.h"
#include "RbException.h"
#include "TypeSpec.h"
#include "ValueRule.h"

#include <cassert>


/** Clone object */
Func_type* Func_type::clone( void ) const {
    
    return new Func_type( *this );
}


/** Execute function */
const RbLanguageObject& Func_type::executeFunction( void ) {
    
    retValue.setValue( args[0].getVariable().getValue().getTypeSpec().getType() );
    
    return retValue;
}


/** Get argument rules */
const ArgumentRules& Func_type::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ValueRule( "variable",   RbObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_type::getClassName(void) { 
    
    static std::string rbClassName = "type function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_type::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_type::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_type::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbString::getClassTypeSpec();
    
    return returnTypeSpec;
}

