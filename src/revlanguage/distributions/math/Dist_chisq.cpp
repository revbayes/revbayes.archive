#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ChisqDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_chisq.h"
#include "Natural.h"
#include "Probability.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_chisq::Dist_chisq(void) : PositiveContinuousDistribution()
{
    
}


Dist_chisq::~Dist_chisq(void)
{
    
}



Dist_chisq* Dist_chisq::clone( void ) const
{
    
    return new Dist_chisq(*this);
}


RevBayesCore::ChisqDistribution* Dist_chisq::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<int>*    df = static_cast<const Natural     &>( degrees->getRevObject() ).getDagNode();
    RevBayesCore::ChisqDistribution* d  = new RevBayesCore::ChisqDistribution( df );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_chisq::getClassType(void)
{
    
    static std::string revType = "Dist_chisq";
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_chisq::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_chisq::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "chisq";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_chisq::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distMemberRules.push_back( new ArgumentRule( "df", Natural::getClassTypeSpec(), "The degrees of freedom.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_chisq::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    return ts;
}


/** Print value for user */
void Dist_chisq::printValue(std::ostream& o) const
{
    
    o << "chisq(p=";
    if ( degrees != NULL )
    {
        o << degrees->getName();
    }
    else
    {
        o << "?";
    }
    
    o << ")";
}


/** Set a member variable */
void Dist_chisq::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "df" )
    {
        degrees = var;
    }
    else
    {
        TypedDistribution<RealPos>::setConstParameter(name, var);
    }
}
