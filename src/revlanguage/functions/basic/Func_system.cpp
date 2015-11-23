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
        
        argumentRules.push_back( new ArgumentRule( "command", RlString::getClassTypeSpec(), "The system command to execute.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
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


/**
 * Get the primary Rev name for this function.
 */
std::string Func_system::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "system";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_system::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_system::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Run a system command." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_system::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    details.push_back( "This function will delegate the command to the system. In that way, the function works as an interface to the shell." );
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_system::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "# We can execute any command just as if you are using a terminal\n";
    example += "system(\"ls\")\n";
    example += "system(\"pwd\")\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_system::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_system::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_system::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Execute a system command.";
    
    return title;
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

