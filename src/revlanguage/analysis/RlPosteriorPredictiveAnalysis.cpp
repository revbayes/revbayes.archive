#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Natural.h"
#include "PosteriorPredictiveAnalysis.h"
#include "RbException.h"
#include "RlMonteCarloAnalysis.h"
#include "RlPosteriorPredictiveAnalysis.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

PosteriorPredictiveAnalysis::PosteriorPredictiveAnalysis() : WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveAnalysis>()
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
PosteriorPredictiveAnalysis* PosteriorPredictiveAnalysis::clone(void) const
{
    
    return new PosteriorPredictiveAnalysis(*this);
}


void PosteriorPredictiveAnalysis::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::MonteCarloAnalysis&         s     = static_cast<const MonteCarloAnalysis &>( sampler->getRevObject() ).getValue();
    const std::string&                              dir   = static_cast<const RlString &>( directory->getRevObject() ).getValue();
    
    value = new RevBayesCore::PosteriorPredictiveAnalysis( s, dir );
    
}


/* Map calls to member methods */
RevPtr<RevVariable> PosteriorPredictiveAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
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
const std::string& PosteriorPredictiveAnalysis::getClassType(void)
{
    
    static std::string revType = "PosteriorPredictiveAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& PosteriorPredictiveAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PosteriorPredictiveAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& PosteriorPredictiveAnalysis::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("sampler"    , MonteCarloAnalysis::getClassTypeSpec()                   , ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule("directory"  , RlString::getClassTypeSpec()                , ArgumentRule::BY_VALUE ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& PosteriorPredictiveAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PosteriorPredictiveAnalysis::printValue(std::ostream &o) const
{
    
    o << "PowerPosterior";
}


/** Set a member variable */
void PosteriorPredictiveAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "sampler")
    {
        sampler = var;
    }
    else if ( name == "directory")
    {
        directory = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
    
}
