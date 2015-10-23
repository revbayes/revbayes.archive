/**
 * @file
 * This file contains the implementation of Func_clear, which is
 * the function used to clear the workspace.
 *
 * @brief Implementation of Func_source
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
#include "Ellipsis.h"
#include "Func_clear.h"
#include "RbException.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_clear::Func_clear( void ) : Procedure()
{
    
}


/** Clone object */
Func_clear* Func_clear::clone( void ) const {
    
    return new Func_clear( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_clear::execute( void )
{
    
    // we clear the entire workspace if there were no arguments
    if ( args.size() == 0 )
    {
        Workspace::userWorkspace().clear();
    }
    else
    {
        for (size_t i = 0; i < args.size(); ++i) 
        {
            Workspace::userWorkspace().eraseVariable( args[i].getVariable() );
        }
    }
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_clear::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new Ellipsis( "Variables to remove.", RevObject::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_clear::getClassType(void) { 
    
    static std::string revType = "Func_clear";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_clear::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_clear::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_clear::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

