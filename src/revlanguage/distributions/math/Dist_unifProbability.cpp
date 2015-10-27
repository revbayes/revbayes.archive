#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_unifProbability.h"
#include "Probability.h"
#include "UniformDistribution.h"

using namespace RevLanguage;

Dist_unifProbability::Dist_unifProbability() : TypedDistribution<Probability>()
{
    
}


Dist_unifProbability::~Dist_unifProbability()
{
    
}



Dist_unifProbability* Dist_unifProbability::clone( void ) const
{
    
    return new Dist_unifProbability(*this);
}


RevBayesCore::UniformDistribution* Dist_unifProbability::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const Probability &>( lower->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const Probability &>( upper->getRevObject() ).getDagNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_unifProbability::getClassType(void)
{
    
    static std::string revType = "Dist_unifProbability";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_unifProbability::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Probability>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_unifProbability::getParameterRules(void) const
{
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    
        distUnifMemberRules.push_back( new ArgumentRule( "lower", Probability::getClassTypeSpec(), "Lower bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper", Probability::getClassTypeSpec(), "Upper bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& Dist_unifProbability::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_unifProbability::printValue(std::ostream& o) const {
    
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
void Dist_unifProbability::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
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
        TypedDistribution<Probability>::setConstParameter(name, var);
    }
}
