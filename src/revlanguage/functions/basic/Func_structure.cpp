#include "Argument.h"
#include "ArgumentRule.h"
#include "DagNode.h"
#include "Func_structure.h"
#include "RbException.h"
#include "RbOptions.h"
#include "RlBoolean.h"
#include "RlUserInterface.h"
#include "RlUtils.h"
#include "TypeSpec.h"

using namespace RevLanguage;

/** Default constructor */
Func_structure::Func_structure( void ) : Procedure()
{
    
}


/** Clone object */
Func_structure* Func_structure::clone( void ) const
{
    
    return new Func_structure( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_structure::execute( void )
{
    
    std::ostringstream o;
    
    bool verbose = static_cast< const RlBoolean &>( args[1].getVariable()->getRevObject() ).getValue();

    // get a reference to the variable from which we want to know the struct
    RevPtr<RevVariable> &the_var = args[0].getVariable();
    
    o << std::endl;
    if ( verbose == true )
    {
        
        // print the name and the memory address
        if ( the_var->getName() == "" )
        {
            o << "_variable     = <unnamed> <" << the_var << ">" << std::endl;
        }
        else
        {
            o << "_variable     = " << the_var->getName() << " <" << the_var << ">" << std::endl;
        }
        
    }
    else
    {
        
        // print only the name
        if ( the_var->getName() == "" )
        {
            o << "_variable     = <unnamed>" << std::endl;
        }
        else
        {
            o << "_variable     = " << the_var->getName() << std::endl;
        }
        
    }
    
    if ( the_var->isWorkspaceVariable() && verbose == true )
    {
        o << "_varType      = workspace (control)" << std::endl;
    }
    else if ( the_var->isReferenceVariable() && verbose == true  )
    {
        o << "_varType      = reference" << std::endl;
        o << "_refVar       = " << the_var->getName() << " <" << the_var << ">" << std::endl;
    }
    
    if ( the_var->getRevObject().isModelObject() == true )
    {
        the_var->getRevObject().getDagNode()->printStructureInfo( o, verbose );
    }
    
    o << std::endl;

    RBOUT( o.str() );

    // no return value
    return NULL;
}


/** Get argument rules */
const ArgumentRules& Func_structure::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x"      , RevObject::getClassTypeSpec(), "The variable.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "verbose", RlBoolean::getClassTypeSpec(), "Do you want all the information?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_structure::getClassType(void)
{
    
    static std::string revType = "Func_type";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_structure::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the alternative Rev names (aliases) for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Func_structure::getFunctionNameAliases( void ) const
{
    // create alternative constructor function names variable that is the same for all instance of this class
    std::vector<std::string> a_names;
    a_names.push_back( "str" );
    
    return a_names;
}


/**
 * Get the author(s) of this function so they can receive credit (and blame) for it.
 */
std::vector<std::string> Func_structure::getHelpAuthor(void) const
{
    // create a vector of authors for this function
    std::vector<std::string> authors;
    authors.push_back( "Sebastian Hoehna" );
    
    return authors;
}


/**
 * Get the (brief) description for this function
 */
std::vector<std::string> Func_structure::getHelpDescription(void) const
{
    // create a variable for the description of the function
    std::vector<std::string> descriptions;
    descriptions.push_back( "Shows all the information about a given variable." );
    
    return descriptions;
}


/**
 * Get the more detailed description of the function
 */
std::vector<std::string> Func_structure::getHelpDetails(void) const
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
std::string Func_structure::getHelpExample(void) const
{
    // create an example as a single string variable.
    std::string example = "";
    example += "# create a variable\n";
    example += "a <- 1\n";
    example += "b := exp(a)\n";
    example += "# now create a deterministic variable which will be a child of 'b'\n";
    example += "c := ln(b)\n";
    example += "# now create a constant variable which will not be a child of 'b'\n";
    example += "d <- ln(b)\n";
    example += "\n";
    example += "str(b)\n";
    
    return example;
}


/**
 * Get some references/citations for this function
 *
 */
std::vector<RevBayesCore::RbHelpReference> Func_structure::getHelpReferences(void) const
{
    // create an entry for each reference
    std::vector<RevBayesCore::RbHelpReference> references;
    
    
    return references;
}


/**
 * Get the names of similar and suggested other functions
 */
std::vector<std::string> Func_structure::getHelpSeeAlso(void) const
{
    // create an entry for each suggested function
    std::vector<std::string> see_also;
    see_also.push_back( "type" );
    
    
    return see_also;
}


/**
 * Get the title of this help entry
 */
std::string Func_structure::getHelpTitle(void) const
{
    // create a title variable
    std::string title = "The structure of a variable";
    
    return title;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_structure::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "structure";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_structure::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_structure::getReturnType( void ) const
{
    
    static TypeSpec returnTypeSpec = RlUtils::Void;
    
    return returnTypeSpec;
}

