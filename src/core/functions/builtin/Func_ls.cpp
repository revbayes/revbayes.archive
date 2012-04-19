/**
 * @file
 * This file contains the implementation of Func_ls, which is
 * the function used to list the content of the workspace.
 *
 * @brief Implementation of Func_ls
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

#include "RbBoolean.h"
#include "DAGNode.h"
#include "Func_ls.h"
#include "Model.h"
#include "RbException.h"
#include "RbNullObject.h"
#include "RbUtil.h"
#include "RbString.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "Workspace.h"

#include <fstream>


/** Clone object */
Func_ls* Func_ls::clone( void ) const {

    return new Func_ls( *this );
}


/** Execute function */
const RbLanguageObject& Func_ls::executeFunction( void ) {

    /* Open file */
    const bool printAll = static_cast<const RbBoolean&>( args[0].getVariable().getValue() ).getValue();
    
    std::cout << "User workspace:" << std::endl;
    std::cout << "===============" << std::endl;
    std::cout << std::endl;

    Workspace::userWorkspace().printValue( std::cout );
    std::cout << std::endl;
    
    if ( printAll == true ) {
        
        std::cout << "Global workspace:" << std::endl;
        std::cout << "===============" << std::endl;
        std::cout << std::endl;

        Workspace::globalWorkspace().printValue( std::cout );
        std::cout << std::endl;
    }

    return RbNullObject::getInstance();
}


/** Get argument rules */
const ArgumentRules& Func_ls::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if ( !rulesSet ) {

        argumentRules.push_back( new ValueRule( "all", new RbBoolean( false ) ) );
        rulesSet = true;
    }

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_ls::getClassName(void) { 
    
    static std::string rbClassName = "Ls function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_ls::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_ls::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_ls::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RbVoid_name;
    return returnTypeSpec;
}

