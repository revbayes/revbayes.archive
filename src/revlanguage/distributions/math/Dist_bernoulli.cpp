#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BernoulliDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_bernoulli.h"
#include "Probability.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_bernoulli::Dist_bernoulli() : TypedDistribution<Natural>() {
    
    setGuiDistributionName("Bernoulli");
    setGuiDistributionToolTip("Bernoulli distribution for 0 or 1 random variables");
}


Dist_bernoulli::~Dist_bernoulli() {
    
}



Dist_bernoulli* Dist_bernoulli::clone( void ) const {
    
    return new Dist_bernoulli(*this);
}


RevBayesCore::BernoulliDistribution* Dist_bernoulli::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* q   = static_cast<const Probability &>( p->getRevObject() ).getDagNode();
    RevBayesCore::BernoulliDistribution* d  = new RevBayesCore::BernoulliDistribution( q );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_bernoulli::getClassType(void) { 
    
    static std::string revType = "Dist_bernoulli";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_bernoulli::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_bernoulli::getParameterRules(void) const
{
    
    static MemberRules distBernMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distBernMemberRules.push_back( new ArgumentRule( "p", Probability::getClassTypeSpec(), "The probability of success.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distBernMemberRules;
}


const TypeSpec& Dist_bernoulli::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_bernoulli::printValue(std::ostream& o) const {
    
    o << "Bernoulli(p=";
    if ( p != NULL ) {
        o << p->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_bernoulli::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
        
    if ( name == "p" ) 
    {
        p = var;
    }
    else
    {
        TypedDistribution<Natural>::setConstParameter(name, var);
    }
}
