#include "Argument.h"
#include "ArgumentRule.h"
#include "Func_ls.h"
#include "RbException.h"
#include "RlBoolean.h"
#include "RlUtils.h"
#include "TypeSpec.h"
#include "RlUserInterface.h"
#include "Workspace.h"

#include <fstream>

using namespace RevLanguage;

/** Default constructor */
Func_ls::Func_ls( void ) : Procedure()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_ls* Func_ls::clone( void ) const
{
    
    return new Func_ls( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_ls::execute( void )
{
    
    bool printAll = static_cast<const RlBoolean &>( args[0].getVariable()->getRevObject() ).getValue();
    
    std::stringstream s;

    VariableTable variables = VariableTable();
    
    if ( printAll )
    {
        const VariableTable& globalVars = Workspace::globalWorkspace().getVariableTable();
        variables.insert( globalVars.begin(), globalVars.end() );
    }
    
    const VariableTable& userVars = env->getVariableTable();
    variables.insert( userVars.begin(), userVars.end() );
    
    if ( !variables.empty() )
    {
        RBOUT( "\n" );
        RBOUT( "Variable table:" );
        RBOUT( "===============" );
        
        s.str("");
        VariableTable::const_iterator it;
        std::ostringstream t1, t2;
        for ( it = variables.begin(); it != variables.end(); ++it )
        {
            t1.str("");
            t1 << (*it).first << " = ";
            t2.str("");
            (*it).second->printValue( t2, true );
            s << t1.str() << StringUtilities::oneLiner( t2.str(), 75 - t1.str().size() ) << std::endl;
        }
        
        s << std::endl;
        RBOUT( s.str() );
    }
    
    s.str("");
    Workspace::userWorkspace().getFunctionTable().printValue( s, printAll );
    if ( s.str().size() > 0 )
    {
        RBOUT( "\n" );
        RBOUT( "Function table:" );
        RBOUT( "===============" );
        
        s << std::endl;
        RBOUT( s.str() );
    }
    
    TypeTable types = TypeTable();

    if ( printAll )
    {
        const TypeTable& globalTypes = Workspace::globalWorkspace().getTypeTable();
        types.insert( globalTypes.begin(), globalTypes.end() );
    }
    
    const TypeTable& userTypes = Workspace::userWorkspace().getTypeTable();
    types.insert( userTypes.begin(), userTypes.end() );
    
    if ( !types.empty() )
    {
        RBOUT( "\n" );
        RBOUT( "Type table:" );
        RBOUT( "===========" );
        
        s.str("");
        TypeTable::const_iterator it;
        std::ostringstream t;
        for ( it = types.begin(); it != types.end(); ++it )
        {
            t.str("");
            t << (*it).first << " = ";
            t << (*it).second->getTypeSpec();
            s << StringUtilities::oneLiner( t.str(), 75 ) << std::endl;
        }
        
        s << std::endl;
        RBOUT( s.str() );
        
    }
    
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_ls::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        argumentRules.push_back( new ArgumentRule( "all", RlBoolean::getClassTypeSpec(), "Should we print all variables and functions including provided ones by RevBayes?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rules_set = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_ls::getClassType(void)
{
    
    static std::string rev_type = "Func_ls";
    
	return rev_type; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_ls::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_ls::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "ls";
    
    return f_name;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_ls::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_ls::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Show the content of the workspace." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_ls::getHelpDetails(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> details;
    details.push_back( "The list functions shows all the variables in the current workspace. You can also see all the functions available if you use ls(all=TRUE)" );
    
    return details;
}


/**
 * Get an executable and instructive example.
 * These example should help the users to show how this function works but
 * are also used to test if this function still works.
 */
std::string Func_ls::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    
    example += "# now we have an empty workspace\n";
    example += "ls()\n";
    example += "# next wee add a variable\n";
    example += "a <- 1\n";
    example += "# and we can see it\n";
    example += "ls()\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_ls::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_ls::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "clear" );
    see_also.push_back( "exists" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_ls::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "List workspace content";
    
    return title;
}


/** Get type spec */
const TypeSpec& Func_ls::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}


/** Get return type */
const TypeSpec& Func_ls::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

