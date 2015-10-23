
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "StationarityStoppingRule.h"
#include "OptionRule.h"
#include "RlStationarityStoppingRule.h"
#include "Probability.h"
#include "RbException.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * Create the default instance.
 */
StationarityStoppingRule::StationarityStoppingRule(void) : AbstractConvergenceStoppingRule()
{
    
}


/**
 * Clone object
 */
StationarityStoppingRule* StationarityStoppingRule::clone(void) const
{
    
    return new StationarityStoppingRule(*this);
}


void StationarityStoppingRule::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new stopping rule
    double p = static_cast<const Probability &>( prob->getRevObject() ).getValue();
    int fq = static_cast<const Natural &>( frequency->getRevObject() ).getValue();
    const std::string &fn = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    
    RevBayesCore::BurninEstimatorContinuous *burninEst = constructBurninEstimator();
    
    value = new RevBayesCore::StationarityStoppingRule(p, fn, size_t(fq), burninEst);
}


/** Get Rev type of object */
const std::string& StationarityStoppingRule::getClassType(void)
{
    
    static std::string revType = "StationarityStoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& StationarityStoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( AbstractConvergenceStoppingRule::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules */
const MemberRules& StationarityStoppingRule::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "prob", Probability::getClassTypeSpec() , "The significance level.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = AbstractConvergenceStoppingRule::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& StationarityStoppingRule::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void StationarityStoppingRule::printValue(std::ostream &o) const
{
    
    o << "StationarityStoppingRule";
}


/** Set a member variable */
void StationarityStoppingRule::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "prob" )
    {
        prob = var;
    }
    else
    {
        AbstractConvergenceStoppingRule::setConstParameter(name, var);
    }
    
}
