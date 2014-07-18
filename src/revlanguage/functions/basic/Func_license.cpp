/**
 * @file
 * This file contains the implementation of Func_license, which is
 * the function used to provide license information.
 *
 * @brief Implementation of Func_license
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_license.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_license::Func_license( void ) : Function() {
    
}


/** Clone object */
Func_license* Func_license::clone( void ) const {
    
    return new Func_license( *this );
}


/** Execute function */
RevPtr<Variable> Func_license::execute( void ) {
    
    
    RBOUT( "\n" );
    RBOUT( "License info coming soon..." );
    RBOUT( "\n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_license::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_license::getClassType(void) {
    
    static std::string revType = "Func_license";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_license::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_license::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_license::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

