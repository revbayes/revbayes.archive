
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "Monitor.h"
#include "OptionRule.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlMonitor.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Monitor::Monitor(void) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>()
{
    
    // add method for call "addVariable" as a function
    ArgumentRules* addArgRules = new ArgumentRules();
    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}


Monitor::Monitor(RevBayesCore::Monitor *m) : WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>( m )
{
    
    // add method for call "addVariable" as a function
    ArgumentRules* addArgRules = new ArgumentRules();
    addArgRules->push_back( new ArgumentRule( "x" , RevObject::getClassTypeSpec(), "A variable you want to monitor.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "addVariable", RlUtils::Void, addArgRules) );
    
}



RevPtr<RevVariable> Monitor::executeMethod(const std::string& name, const std::vector<Argument>& args, bool &found)
{
    
    if ( name == "addVariable" )
    {
        found = true;
        
        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
        
        RevBayesCore::Monitor *m = static_cast<RevBayesCore::Monitor*>(this->value);
        m->addVariable( node );
        
        return NULL;
    }
    else if ( name == "removeVariable" )
    {
        found = true;
        
        RevBayesCore::DagNode* node = args[0].getVariable()->getRevObject().getDagNode();
        
        RevBayesCore::Monitor *m = static_cast<RevBayesCore::Monitor*>(this->value);
        m->removeVariable( node );
        
        return NULL;
    }
    
    return WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Monitor::getClassType(void)
{
    
    static std::string revType = "Monitor";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Monitor::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::Monitor>::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the aliases of the Rev name for the constructor function.
 *
 * \return Rev aliases of constructor function.
 */
std::vector<std::string> Monitor::getConstructorFunctionAliases( void ) const
{
    // create a constructor function name alias variable that is the same for all instance of this class
    std::vector<std::string> aliases;
    
    std::vector<std::string> monitor_aliases = getMonitorAliases();
    for (size_t i=0; i < monitor_aliases.size(); ++i)
    {
        std::string tmp = monitor_aliases[i];
        std::string c_name = "mn" + StringUtilities::firstCharToUpper( tmp );
        
        aliases.push_back( c_name );
    }
    
    return aliases;
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Monitor::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string tmp = getMonitorName();
    std::string c_name = "mn" + StringUtilities::firstCharToUpper( tmp );
    
    return c_name;
}


/** Get the help entry for this class */
RevBayesCore::RbHelpMonitor* Monitor::getHelpEntry( void ) const
{
    // create the help function entry that we will fill with some values
    RevBayesCore::RbHelpMonitor *help = new RevBayesCore::RbHelpMonitor();
    RevBayesCore::RbHelpMonitor &helpEntry = *help;
    
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


/** Get type spec */
void Monitor::printValue(std::ostream &o, bool user) const
{
    
    printValue(o);
}

/** Get type spec */
void Monitor::printValue(std::ostream &o) const
{
    
    o << getMonitorName();
}




