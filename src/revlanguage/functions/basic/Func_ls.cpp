/**
 * @file
 * This file contains the implementation of Func_quit, which is
 * the function used to quit RevBayes.
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
#include "Func_ls.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_ls::Func_ls( void ) : Function() {
    
}


/** Clone object */
Func_ls* Func_ls::clone( void ) const {
    
    return new Func_ls( *this );
}


/** Execute function */
RevPtr<Variable> Func_ls::execute( void ) {
    
    bool printAll = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getValue();
    
    RBOUT( "User workspace:" );
    RBOUT( "===============" );
    RBOUT( "\n" );
    
    Workspace::userWorkspace().printValue( std::cout );
    RBOUT( "\n" );
    
    if ( printAll == true ) {
        
        RBOUT( "Global workspace:" );
        RBOUT( "=================" );
        RBOUT( "\n" );
        
        Workspace::globalWorkspace().printValue( std::cout );
        RBOUT( "\n" );
    }

    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_ls::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", true, RlBoolean::getClassTypeSpec(), new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_ls::getClassName(void) { 
    
    static std::string rbClassName = "Func_ls";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_ls::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_ls::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_ls::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

