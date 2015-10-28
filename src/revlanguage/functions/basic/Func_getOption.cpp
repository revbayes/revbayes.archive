#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_getOption.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_getOption::Func_getOption( void ) : Procedure()
{
    
}


/** Clone object */
Func_getOption* Func_getOption::clone( void ) const
{
    
    return new Func_getOption( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_getOption::execute( void )
{
    
    const std::string &key = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    
    RbSettings& s = RbSettings::userSettings();
    std::string value = s.getOption( key );
    
    return RevPtr<RevVariable>( new RevVariable( new RlString( value ) ) );
}


/** Get argument rules */
const ArgumentRules& Func_getOption::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "key", RlString::getClassTypeSpec(), "The key-identifier for the option.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_getOption::getClassType(void)
{
    
    static std::string revType = "Func_getOption";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_getOption::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_getOption::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "getOption";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_getOption::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_getOption::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

