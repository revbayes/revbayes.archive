#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Natural.h"
#include "ValidationAnalysis.h"
#include "RbException.h"
#include "RlMonteCarloAnalysis.h"
#include "RlValidationAnalysis.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

ValidationAnalysis::ValidationAnalysis() : WorkspaceToCoreWrapperObject<RevBayesCore::ValidationAnalysis>()
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), "The number of generation to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
    
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), "The number of generations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), "The number of iterations after which we tune the parameters of the moves.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "burnin", RlUtils::Void, burninArgRules) );
    
    ArgumentRules* summarizeArgRules = new ArgumentRules();
//    summarizeArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), "The number of generation to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "summarize", RlUtils::Void, summarizeArgRules) );
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
ValidationAnalysis* ValidationAnalysis::clone(void) const
{
    
    return new ValidationAnalysis(*this);
}


void ValidationAnalysis::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::MonteCarloAnalysis&         s   = static_cast<const MonteCarloAnalysis &>( sampler->getRevObject() ).getValue();
    int                                             n   = static_cast<const Natural &>( simulations->getRevObject() ).getValue();
    
    value = new RevBayesCore::ValidationAnalysis( s, size_t(n) );
    
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string ValidationAnalysis::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "validationAnalysis";
    
    return c_name;
}


/* Map calls to member methods */
RevPtr<RevVariable> ValidationAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->runAll( size_t(gen) );
        
        return NULL;
    }
    else if (name == "burnin")
    {
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        int tuningInterval = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getValue();
        value->burnin( size_t(gen), size_t(tuningInterval) );
        
        return NULL;
    }
    else if (name == "summarize")
    {
        found = true;
        
        value->summarizeAll();
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& ValidationAnalysis::getClassType(void)
{
    
    static std::string revType = "ValidationAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& ValidationAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::ValidationAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& ValidationAnalysis::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("sampler"       , MonteCarloAnalysis::getClassTypeSpec(), "The template Monte Carlo sampler instance.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("simulations"   , Natural::getClassTypeSpec()           , "How many replicate simulations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& ValidationAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void ValidationAnalysis::printValue(std::ostream &o) const
{
    
    o << "ValidationAnalysis";
}


/** Set a member variable */
void ValidationAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "sampler")
    {
        sampler = var;
    }
    else if ( name == "simulations")
    {
        simulations = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
    
}
