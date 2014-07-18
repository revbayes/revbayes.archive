/**
 * @file
 * This file contains the implementation of Func_help, which is
 * the function used to provide basic help.
 *
 * @brief Implementation of Func_help
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_help.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_help::Func_help( void ) : Function() {
    
}


/** Clone object */
Func_help* Func_help::clone( void ) const {
    
    return new Func_help( *this );
}


/** Execute function */
RevPtr<Variable> Func_help::execute( void ) {
    
    
    RBOUT( "\n" );
    RBOUT( "Help coming soon..." );
    RBOUT( "\n" );

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_help::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_help::getClassType(void) {
    
    static std::string revType = "Func_help";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_help::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_help::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_help::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

