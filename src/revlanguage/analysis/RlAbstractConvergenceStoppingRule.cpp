
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "EssMax.h"
#include "AbstractConvergenceStoppingRule.h"
#include "OptionRule.h"
#include "RlAbstractConvergenceStoppingRule.h"
#include "RealPos.h"
#include "RbException.h"
#include "RlString.h"
#include "SemMin.h"
#include "TypeSpec.h"


using namespace RevLanguage;

/**
 * Default constructor.
 * Create the default instance.
 */
AbstractConvergenceStoppingRule::AbstractConvergenceStoppingRule(void) : StoppingRule()
{
    
}


RevBayesCore::BurninEstimatorContinuous* AbstractConvergenceStoppingRule::constructBurninEstimator( void )
{
    
    // create a new Burnin Estimator instance
    const std::string &bm = static_cast<const RlString &>( burninMethod->getRevObject() ).getValue();
    
    RevBayesCore::BurninEstimatorContinuous *burninEst = NULL;
    
    if ( bm == "ESS" )
    {
        burninEst = new RevBayesCore::EssMax();
    }
    else if ( bm == "SEM" )
    {
        burninEst = new RevBayesCore::SemMin();
    }
    else
    {
        throw RbException("Unknown burnin estimation method");
    }
    
    return burninEst;
}


/** Get Rev type of object */
const std::string& AbstractConvergenceStoppingRule::getClassType(void)
{
    
    static std::string revType = "AbstractConvergenceStoppingRule";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& AbstractConvergenceStoppingRule::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( StoppingRule::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules */
const MemberRules& AbstractConvergenceStoppingRule::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "filename" , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        memberRules.push_back( new ArgumentRule( "frequency", Natural::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(10000) ) );
        
        std::vector<std::string> bMethods;
        bMethods.push_back( "ESS" );
        bMethods.push_back( "SEM" );
        //        optionsUnits.push_back( "fixed" );
        memberRules.push_back( new OptionRule( "burninMethod", new RlString("ESS"), bMethods ) );
        
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& AbstractConvergenceStoppingRule::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Set a member variable */
void AbstractConvergenceStoppingRule::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "burninMethod" )
    {
        burninMethod = var;
    }
    else if ( name == "filename" )
    {
        filename = var;
    }
    else if ( name == "frequency" )
    {
        frequency = var;
    }
    else
    {
        StoppingRule::setConstParameter(name, var);
    }
    
}
