#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlDistribution.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;


/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Distribution::Distribution() : RevObject() 
{
    
}


/**
 * Default destructor.
 * 
 * The default destructor does nothing except deleting the object.
 */
Distribution::~Distribution() 
{
    
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Distribution::getClassType(void) 
{ 
    
    static std::string revType = "Distribution";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Distribution::getClassTypeSpec(void) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get the help entry for this class */
RevBayesCore::RbHelpDistribution* Distribution::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpDistribution *help = new RevBayesCore::RbHelpDistribution();
    RevBayesCore::RbHelpDistribution &helpEntry = *help;
    
//    // name
//    helpEntry.setName( "name" );
    
//    // aliases
//    std::vector<std::string> aliases = std::vector<std::string>();
//    //    aliases.push_back( "alias" );
//    helpEntry.setAliases( aliases );
    
    // title
    helpEntry.setTitle( getHelpTitle() );
    
    // description
    helpEntry.setDescription( getHelpDescription() );
    
    
    
    // create the constructor
    RevBayesCore::RbHelpFunction help_constructor = RevBayesCore::RbHelpFunction();

    // usage
    help_constructor.setUsage( getConstructorUsage() );
    
    // arguments
    const MemberRules& rules = getParameterRules();
    std::vector<RevBayesCore::RbHelpArgument> arguments = std::vector<RevBayesCore::RbHelpArgument>();
    
    for ( size_t i=0; i<rules.size(); ++i )
    {
        const ArgumentRule &the_rule = rules[i];
        
        RevBayesCore::RbHelpArgument argument = RevBayesCore::RbHelpArgument();
        
        argument.setLabel( the_rule.getArgumentLabel() );
        argument.setDescription( the_rule.getArgumentDescription() );
        
        std::string type = "<any>";
        if ( the_rule.getArgumentDagNodeType() == ArgumentRule::CONSTANT )
        {
            type = "<constant>";
        }
        else if ( the_rule.getArgumentDagNodeType() == ArgumentRule::STOCHASTIC )
        {
            type = "<stochastic>";
        }
        else if ( the_rule.getArgumentDagNodeType() == ArgumentRule::DETERMINISTIC )
        {
            type = "<deterministic>";
        }
        argument.setArgumentDagNodeType( type );
        
        std::string passing_method = "value";
        if ( the_rule.getEvaluationType() == ArgumentRule::BY_CONSTANT_REFERENCE )
        {
            passing_method = "const reference";
        }
        else if ( the_rule.getEvaluationType() == ArgumentRule::BY_REFERENCE )
        {
            passing_method = "reference";
        }
        argument.setArgumentPassingMethod(  passing_method );
        
        argument.setValueType( the_rule.getArgumentTypeSpec()[0].getType() );
        std::stringstream ss;
        the_rule.getDefaultVariable().getRevObject().printValue( ss, true);
        argument.setDefaultValue( ss.str() );
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        std::string option = std::string( "o" );
        options.push_back( option );
        argument.setOptions( options );
        
        // add the argument to the argument list
        arguments.push_back( argument );
    }
    
    help_constructor.setArguments( arguments );
    
    // return value
    help_constructor.setReturnType( getVariableTypeSpec().getType() );
    
    // details
    help_constructor.setDetails( getConstructorDetails() );
    
    // example
    help_constructor.setExample( getConstructorExample() );
    
    //
    std::vector<RevBayesCore::RbHelpFunction> constructors;
    constructors.push_back( help_constructor );
    helpEntry.setConstructors( constructors );
    
    
    helpEntry.setReferences( getHelpReferences() );
    
    // author
    helpEntry.setAuthor( getHelpAuthor() );
    
    // see also
    helpEntry.setSeeAlso( getHelpSeeAlso() );
    
    return help;
    
}


/**
 * Print the value of this object for the user.
 *
 * There is not much of a value to print for a distribution. 
 * Thus, we simply print the name of the distribution.
 *
 * \param[in]    the stream to which to print.
 */
void Distribution::printValue(std::ostream &o) const 
{
    o << getClassType() << "(...)" << std::endl;
}
