
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "Move.h"
#include "OptionRule.h"
#include "RevObject.h"
#include "RbException.h"
#include "RealPos.h"
#include "RlMove.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move::Move(void) : WorkspaceToCoreWrapperObject<RevBayesCore::Move>()
{
    
}


/** Get Rev type of object */
const std::string& Move::getClassType(void)
{
    
    static std::string revType = "Move";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::Move>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the aliases of the Rev name for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Move::getConstructorFunctionAliases( void ) const
{
    // create a constructor function name alias variable that is the same for all instance of this class
    std::vector<std::string> aliases;
    
    std::vector<std::string> move_aliases = getMoveAliases();
    for (size_t i=0; i < move_aliases.size(); ++i)
    {
        std::string tmp = move_aliases[i];
        std::string c_name = "mv" + StringUtilities::firstCharToUpper( tmp );
        
        aliases.push_back( c_name );
    }
    
    return aliases;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string tmp = getMoveName();
    std::string c_name = "mv" + StringUtilities::firstCharToUpper( tmp );
    
    return c_name;
}


/** Get the help entry for this class */
RevBayesCore::RbHelpMove* Move::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpMove *help = new RevBayesCore::RbHelpMove();
    RevBayesCore::RbHelpMove &helpEntry = *help;
    
    // name
    helpEntry.setName( getConstructorFunctionName() );
    
    // aliases
    std::vector<std::string> aliases = getConstructorFunctionAliases();
    helpEntry.setAliases( aliases );
    
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
        
        if ( the_rule.hasDefault() )
        {
            std::stringstream ss;
            the_rule.getDefaultVariable().getRevObject().printValue( ss, true);
            argument.setDefaultValue( ss.str() );
        }
        else
        {
            argument.setDefaultValue( "" );
        }
        
        // loop options
        std::vector<std::string> options = std::vector<std::string>();
        const OptionRule *opt_rule = dynamic_cast<const OptionRule*>( &the_rule );
        if ( opt_rule != NULL )
        {
            options = opt_rule->getOptions();
        }
        argument.setOptions( options );
        
        // add the argument to the argument list
        arguments.push_back( argument );
    }
    
    help_constructor.setArguments( arguments );
    
    // return value
    help_constructor.setReturnType( getClassType() );
    
    // details
    help_constructor.setDetails( getConstructorDetails() );
    
    // example
    help_constructor.setExample( getConstructorExample() );
    
    //
    std::vector<RevBayesCore::RbHelpFunction> constructors;
    constructors.push_back( help_constructor );
    helpEntry.setConstructors( constructors );
    
    helpEntry.setMethods( getHelpMethods() );
    
    helpEntry.setReferences( getHelpReferences() );
    
    // author
    helpEntry.setAuthor( getHelpAuthor() );
    
    // see also
    helpEntry.setSeeAlso( getHelpSeeAlso() );
    
    return help;
    
}



/** Return member rules */
const MemberRules& Move::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        moveMemberRules.push_back( new ArgumentRule( "weight", RealPos::getClassTypeSpec(), "The weight how often on average this move will be used per iteration.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RealPos( 1.0 ) ) );
        
        /* Inherit variables from WorkspaceObject, put them last */
        const MemberRules& inheritedRules = WorkspaceObject::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() ); 
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}


/** Set a member variable */
void Move::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "weight" ) 
    {
        weight = var;
    }
    else 
    {
        RevObject::setConstParameter(name, var);
    }
}

