#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Natural.h"
#include "BootstrapAnalysis.h"
#include "RbException.h"
#include "RlMaximumLikelihoodAnalysis.h"
#include "RlBootstrapAnalysis.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

BootstrapAnalysis::BootstrapAnalysis() : WorkspaceToCoreWrapperObject<RevBayesCore::BootstrapAnalysis>()
{
    
    ArgumentRules* runArgRules = new ArgumentRules();
    runArgRules->push_back( new ArgumentRule("generations", Natural::getClassTypeSpec(), "The number of generation to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "run", RlUtils::Void, runArgRules) );
        
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
BootstrapAnalysis* BootstrapAnalysis::clone(void) const
{
    
    return new BootstrapAnalysis(*this);
}


void BootstrapAnalysis::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    const RevBayesCore::MaximumLikelihoodAnalysis&  s   = static_cast<const MaximumLikelihoodAnalysis &>( sampler->getRevObject() ).getValue();
    int                                             n   = static_cast<const Natural &>( simulations->getRevObject() ).getValue();
    
    value = new RevBayesCore::BootstrapAnalysis( s, size_t(n) );
    
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string BootstrapAnalysis::getConstructorFunctionName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "BootstrapAnalysis";
    
    return c_name;
}


/* Map calls to member methods */
RevPtr<RevVariable> BootstrapAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "run")
    {
        
        found = true;
        
        // get the member with give index
        int gen = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getValue();
        value->runAll( size_t(gen) );
        
        return NULL;
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& BootstrapAnalysis::getClassType(void)
{
    
    static std::string revType = "BootstrapAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& BootstrapAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::BootstrapAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& BootstrapAnalysis::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule("estimator"     , MaximumLikelihoodAnalysis::getClassTypeSpec(), "The maximum likelihood estimation object.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule("simulations"   , Natural::getClassTypeSpec()                  , "How many replicate simulations to run.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


/** Get type spec */
const TypeSpec& BootstrapAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void BootstrapAnalysis::printValue(std::ostream &o) const
{
    
    o << "BootstrapAnalysis";
}


/** Set a member variable */
void BootstrapAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
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
