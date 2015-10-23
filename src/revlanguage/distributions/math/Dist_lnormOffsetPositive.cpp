#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_lnormOffsetPositive.h"
#include "LognormalWithOffsetDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_lnormOffsetPositive::Dist_lnormOffsetPositive() : PositiveContinuousDistribution()
{
    
}



Dist_lnormOffsetPositive* Dist_lnormOffsetPositive::clone( void ) const
{
    return new Dist_lnormOffsetPositive(*this);
}


RevBayesCore::LognormalWithOffsetDistribution* Dist_lnormOffsetPositive::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m   = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o   = static_cast<const RealPos &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::LognormalWithOffsetDistribution* d  = new RevBayesCore::LognormalWithOffsetDistribution(m, s, o);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_lnormOffsetPositive::getClassType(void)
{
    
    static std::string revType = "Dist_lnormOffsetPositive";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_lnormOffsetPositive::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_lnormOffsetPositive::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        memberRules.push_back( new ArgumentRule( "mean",   Real::getClassTypeSpec()   , "The mean in log-space (observed mean is exp(m)).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "sd"  ,   RealPos::getClassTypeSpec(), "The standard deviation in log-space."             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        memberRules.push_back( new ArgumentRule( "offset", RealPos::getClassTypeSpec(), "The offset in real-space."                        , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_lnormOffsetPositive::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_lnormOffsetPositive::printValue(std::ostream& o) const
{
    
    o << "lognormal(mean=";
    if ( mean != NULL )
    {
        o << mean->getName();
    }
    else
    {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL )
    {
        o << sd->getName();
    }
    else
    {
        o << "?";
    }
    o << ", offset=";
    if ( offset != NULL )
    {
        o << offset->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_lnormOffsetPositive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "mean" )
    {
        mean = var;
    }
    else if ( name == "sd" )
    {
        sd = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else
    {
        PositiveContinuousDistribution::setConstParameter(name, var);
    }
}
