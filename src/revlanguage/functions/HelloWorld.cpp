/**
 * @file
 * This file contains the implementation of HelloWorld, which is
 * the function used to provide basic help.
 *
 * @brief Implementation of HelloWorld
 *
 * @author Fredrik Ronquist
 * @license GPL version 3
 */

#include "Argument.h"
#include "ArgumentRule.h"
#include "HelloWorld.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
HelloWorld::HelloWorld( void ) : Function() {
    
}


/** Clone object */
HelloWorld* HelloWorld::clone( void ) const {
    
    return new HelloWorld( *this );
}


/** Execute function */
RevPtr<Variable> HelloWorld::execute( void ) {
    
    
    RBOUT( "Hello World!" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& HelloWorld::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& HelloWorld::getClassType(void) {
    
    static std::string revType = "HelloWorld";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& HelloWorld::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& HelloWorld::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& HelloWorld::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

