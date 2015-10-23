#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BetaDistribution.h"
#include "Dist_beta.h"
#include "RealPos.h"
#include "Probability.h"
#include "RlContinuousStochasticNode.h"

using namespace RevLanguage;

Dist_beta::Dist_beta() : TypedDistribution<Probability>()
{
    
    setGuiDistributionName("Beta");
    setGuiDistributionToolTip("Beta distribution for random variables on the interval [0,1]");
}


Dist_beta::~Dist_beta()
{
    
}



Dist_beta* Dist_beta::clone( void ) const
{
    
    return new Dist_beta(*this);
}


RevBayesCore::BetaDistribution* Dist_beta::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* a   = static_cast<const RealPos &>( alpha->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* b   = static_cast<const RealPos &>( beta->getRevObject() ).getDagNode();
    RevBayesCore::BetaDistribution* d       = new RevBayesCore::BetaDistribution(a, b);
    
    return d;
}



Probability* Dist_beta::createRandomVariable(void) const
{
    
    RevBayesCore::ContinuousDistribution* d = createDistribution();
    RevBayesCore::TypedDagNode<double>* rv  = new ContinuousStochasticNode("", d, this->clone() );
    
    return new Probability(rv);
}



/* Get Rev type of object */
const std::string& Dist_beta::getClassType(void) { 
    
    static std::string revType = "Dist_beta";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_beta::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Probability>::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_beta::getParameterRules(void) const {
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distUnifMemberRules.push_back( new ArgumentRule( "alpha", RealPos::getClassTypeSpec(), "The alpha shape parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distUnifMemberRules.push_back( new ArgumentRule( "beta" , RealPos::getClassTypeSpec(), "The beta shape parameter.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& Dist_beta::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_beta::printValue(std::ostream& o) const {
    
    o << "beta(alpha=";
    if ( alpha != NULL ) {
        o << alpha->getName();
    } else {
        o << "?";
    }
    o << ", beta=";
    if ( beta != NULL ) {
        o << beta->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_beta::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
        
    if ( name == "alpha" ) 
    {
        alpha = var;
    }
    else if ( name == "beta" ) 
    {
        beta = var;
    }
    else
    {
        TypedDistribution<Probability>::setConstParameter(name, var);
    }
}
