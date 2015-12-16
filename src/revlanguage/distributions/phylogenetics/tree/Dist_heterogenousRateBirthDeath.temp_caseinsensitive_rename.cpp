#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_heterogeneousRateBirthDeath.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "UniformTimeTreeDistribution.h"

using namespace RevLanguage;


Dist_heterogeneousRateBirthDeath::Dist_heterogeneousRateBirthDeath() : TypedDistribution<TimeTree>()
{
    
}


Dist_heterogeneousRateBirthDeath::~Dist_heterogeneousRateBirthDeath()
{
    
}



Dist_heterogeneousRateBirthDeath* Dist_heterogeneousRateBirthDeath::clone( void ) const
{
    
    return new Dist_heterogeneousRateBirthDeath( *this );
}


RevBayesCore::UniformTimeTreeDistribution* Dist_heterogeneousRateBirthDeath::createDistribution( void ) const
{
    
    // Get the parameters
    RevBayesCore::TypedDagNode<double>* a   = static_cast<const RealPos &>( root_age->getRevObject() ).getDagNode();
    std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    
    RevBayesCore::UniformTimeTreeDistribution*   d = new RevBayesCore::UniformTimeTreeDistribution( a, t );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_heterogeneousRateBirthDeath::getClassType(void)
{
    
    static std::string revType = "Dist_heterogeneousRateBirthDeath";
    
    return revType;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& Dist_heterogeneousRateBirthDeath::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_heterogeneousRateBirthDeath::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "UniformTimeTree";
    
    return d_name;
}


/* Return member rules */
const MemberRules& Dist_heterogeneousRateBirthDeath::getParameterRules(void) const
{
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "rootAge"  , RealPos::getClassTypeSpec()            , "The age of the root.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distMemberRules.push_back( new ArgumentRule( "taxa"     , ModelVector<Taxon>::getClassTypeSpec() , "The taxa used for simulation.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_heterogeneousRateBirthDeath::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void Dist_heterogeneousRateBirthDeath::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "rootAge" )
    {
        root_age = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}

