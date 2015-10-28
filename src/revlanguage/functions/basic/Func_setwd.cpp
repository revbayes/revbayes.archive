#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_setwd.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_setwd::Func_setwd( void ) : Procedure()
{
    
}


/** Clone object */
Func_setwd* Func_setwd::clone( void ) const
{
    
    return new Func_setwd( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_setwd::execute( void )
{
    
    const std::string &wd = static_cast<const RlString &>( args[0].getVariable()->getRevObject() ).getValue();
    
    RbSettings& s = RbSettings::userSettings();
    s.setWorkingDirectory( wd );
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_setwd::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "wd", RlString::getClassTypeSpec(), "The new working directory.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_setwd::getClassType(void)
{
    
    static std::string revType = "Func_setwd";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_setwd::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
const std::string& Func_setwd::getFunctionName( void ) const
{
    // create a static name variable that is the same for all instance of this class
    static std::string f_name = "setwd";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_setwd::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_setwd::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

