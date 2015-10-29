#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_exponentialOffsetPositive.h"
#include "ExponentialWithOffsetDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

Dist_exponentialOffsetPositive::Dist_exponentialOffsetPositive() : PositiveContinuousDistribution() {
    
}


Dist_exponentialOffsetPositive::~Dist_exponentialOffsetPositive() {
    
}



Dist_exponentialOffsetPositive* Dist_exponentialOffsetPositive::clone( void ) const {
    return new Dist_exponentialOffsetPositive(*this);
}


RevBayesCore::ExponentialWithOffsetDistribution* Dist_exponentialOffsetPositive::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<double>* l     = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o     = static_cast<const Real    &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialWithOffsetDistribution* d  = new RevBayesCore::ExponentialWithOffsetDistribution( l, o );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_exponentialOffsetPositive::getClassType(void) {
    
    static std::string revType = "Dist_exponentialOffsetPositive";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_exponentialOffsetPositive::getClassTypeSpec(void) {
    
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
std::string Dist_exponentialOffsetPositive::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "exponential";
    
    return d_name;
}


/** Return member rules (no members) */
const MemberRules& Dist_exponentialOffsetPositive::getParameterRules(void) const
{
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
    
        distExpMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), "The rate parameter ( rate = 1/mean ).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distExpMemberRules.push_back( new ArgumentRule( "offset", RealPos::getClassTypeSpec(), "The offset of the distribution.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& Dist_exponentialOffsetPositive::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_exponentialOffsetPositive::printValue(std::ostream& o) const {
    
    o << " exponential(lambda=";
    if ( lambda != NULL ) {
        o << lambda->getName();
    } else {
        o << "?";
    }
    o << ", offset=";
    if ( offset != NULL ) {
        o << offset->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_exponentialOffsetPositive::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
