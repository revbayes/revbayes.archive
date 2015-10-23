#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_setOption.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_setOption::Func_setOption( void ) : Procedure()
{
    
}


/** Clone object */
Func_setOption* Func_setOption::clone( void ) const
{
    
    return new Func_setOption( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_setOption::execute( void )
{
    
    const std::string &key = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string &value = static_cast<const RlString &>( args[1].getVariable()->getRevObject() ).getValue();
    
    RbSettings& s = RbSettings::userSettings();
    s.setOption( key, value, true );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_setOption::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "key", RlString::getClassTypeSpec(),   "The key-identifier for which to set a new value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        argumentRules.push_back( new ArgumentRule( "value", RlString::getClassTypeSpec(), "The new value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_setOption::getClassType(void)
{
    
    static std::string revType = "Func_setOption";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Func_setOption::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}

/** Get type spec */
const TypeSpec& Func_setOption::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_setOption::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

