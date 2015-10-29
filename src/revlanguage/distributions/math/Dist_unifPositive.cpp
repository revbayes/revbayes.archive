#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_unifPositive.h"
#include "Real.h"
#include "RealPos.h"
#include "UniformDistribution.h"

using namespace RevLanguage;

Dist_unifPositive::Dist_unifPositive() : PositiveContinuousDistribution()
{
    
}


Dist_unifPositive::~Dist_unifPositive() {
    
}



Dist_unifPositive* Dist_unifPositive::clone( void ) const {
    return new Dist_unifPositive(*this);
}


RevBayesCore::UniformDistribution* Dist_unifPositive::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const RealPos &>( lower->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const RealPos &>( upper->getRevObject() ).getDagNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_unifPositive::getClassType(void) { 
    
    static std::string revType = "Dist_unif";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_unifPositive::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_unifPositive::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "Uniform";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_unifPositive::getParameterRules(void) const
{
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distUnifMemberRules.push_back( new ArgumentRule( "lower", RealPos::getClassTypeSpec(), "The lower bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper", RealPos::getClassTypeSpec(), "The upper bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& Dist_unifPositive::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_unifPositive::printValue(std::ostream& o) const {
    
    o << " unif (lower=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ", upper=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_unifPositive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "lower" ) 
    {
        lower = var;
    }
    else if ( name == "upper" ) 
    {
        upper = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
