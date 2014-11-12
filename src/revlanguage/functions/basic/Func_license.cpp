#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_license.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_license::Func_license( void ) : Procedure() {
    
}


/** Clone object */
Func_license* Func_license::clone( void ) const {
    
    return new Func_license( *this );
}


/** Execute function */
RevPtr<Variable> Func_license::execute( void )
{
    RBOUT( "" );
    RBOUT( "RevBayes license" );
    RBOUT( "================" );
    RBOUT( "" );
    RBOUT( "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version." );
    RBOUT( "" );
    RBOUT( "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License (http://www.gnu.org/licenses/) for more details." );
    RBOUT( "" );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_license::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
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

