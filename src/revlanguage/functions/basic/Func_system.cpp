#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_system.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <stdlib.h>

using namespace RevLanguage;

/** Default constructor */
Func_system::Func_system( void ) : Procedure()
{
    
}


/** Clone object */
Func_system* Func_system::clone( void ) const
{
    
    return new Func_system( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_system::execute( void )
{
    
    const char *cmd = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue().c_str();
    
    system( cmd );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_system::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_system::getClassType(void)
{
    
    static std::string revType = "Func_system";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_system::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_system::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_system::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

