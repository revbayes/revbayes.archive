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
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("run", new MemberProcedure( RlUtils::Void, runArgRules) );
    
    ArgumentRules* burninArgRules = new ArgumentRules();
    burninArgRules->push_back( new ArgumentRule("generations"   , Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    burninArgRules->push_back( new ArgumentRule("tuningInterval", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
    methods.addFunction("burnin", new MemberProcedure( RlUtils::Void, burninArgRules) );
    
}


/** Clone object */
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
        
        memberRules.push_back( new ArgumentRule("sampler"       , MonteCarloAnalysis::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("simulations"   , Natural::getClassTypeSpec()           , ArgumentRule::BY_VALUE ) );
        
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
    
    o << "PowerPosterior";
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
