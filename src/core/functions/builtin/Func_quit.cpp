/**
 * @file
 * This file contains the implementation of Func_quit, which is
 * the function used to quit the program.
 *
 * @brief Implementation of Func_quit
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

#include "DAGNode.h"
#include "Func_quit.h"
#include "Model.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "UserInterface.h"
#include "ValueRule.h"

#include <fstream>


/** Clone object */
Func_quit* Func_quit::clone( void ) const {

    return new Func_quit( *this );
}


/** Execute operation */
const RbLanguageObject& Func_quit::execute( void ) {

    /* Ask if user really wants to quit */
    //bool wantToQuit = UserInterface::userInterface().ask("Do you really want to quit"); // TEMP: Getting tired of typing 'y' (JPH)
    bool wantToQuit = true;
	
    if ( wantToQuit )
        throw RbException( RbException::QUIT );

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_quit::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_quit::getClassName(void) { 
    
    static std::string rbClassName = "Quit function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_quit::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_quit::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_quit::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

