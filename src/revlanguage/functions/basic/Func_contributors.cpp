/**
 * @file
 * This file contains the implementation of Func_contributors, which is
 * the function used to provide information on contributors.
 *
 * @brief Implementation of Func_contributors
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_contributors.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_contributors::Func_contributors( void ) : Function() {
    
}


/** Clone object */
Func_contributors* Func_contributors::clone( void ) const {
    
    return new Func_contributors( *this );
}


/** Execute function */
RevPtr<Variable> Func_contributors::execute( void ) {
    
    
    RBOUT( "\n" );
    RBOUT( "More info coming soon..." );
    RBOUT( "\n" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_contributors::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_contributors::getClassType(void) {
    
    static std::string rbClassName = "Func_contributors";
    
	return rbClassName;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_contributors::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass;
}

/** Get type spec */
const TypeSpec& Func_contributors::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_contributors::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

