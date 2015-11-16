#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_getwd.h"
#include "RbException.h"
#include "RbSettings.h"
#include "RlString.h"
#include "RlUtils.h"
#include "TypeSpec.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_getwd::Func_getwd( void ) : Procedure()
{
    
}


/** Clone object */
Func_getwd* Func_getwd::clone( void ) const
{
    
    return new Func_getwd( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_getwd::execute( void )
{
    
    RbSettings& s = RbSettings::userSettings();
    const std::string& wd = s.getWorkingDirectory();
    
    RlString* type = new RlString( wd );
    
    return new RevVariable( type );
}


/** Get argument rules */
const ArgumentRules& Func_getwd::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_getwd::getClassType(void)
{
    
    static std::string revType = "Func_getwd";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_getwd::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_getwd::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "getwd";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_getwd::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_getwd::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Get the current working directory which RevBayes uses." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_getwd::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_getwd::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "# get the current working directory\n";
    example += "getwd()\n";
    example += "\n";
    example += "# let us set a new working directory\n";
    example += "setwd(\"~/Desktop\")\n";
    example += "\n";
    example += "# check the working directory again\n";
    example += "getwd()\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_getwd::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_getwd::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "setwd" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_getwd::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "Get and print the working directory";
    
    return title;
}



/** Get type spec */
const TypeSpec& Func_getwd::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_getwd::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlString::getClassTypeSpec();
    
    return returnTypeSpec;
}

