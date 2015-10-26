#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_logUniform.h"
#include "LogUniformDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_logUniform::Dist_logUniform() : PositiveContinuousDistribution() {
    
}


Dist_logUniform::~Dist_logUniform() {
    
}



Dist_logUniform* Dist_logUniform::clone( void ) const {
    return new Dist_logUniform(*this);
}


RevBayesCore::LogUniformDistribution* Dist_logUniform::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* mi   = static_cast<const Real &>( min->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* ma   = static_cast<const Real &>( max->getRevObject() ).getDagNode();
    RevBayesCore::LogUniformDistribution* d  = new RevBayesCore::LogUniformDistribution( mi, ma );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_logUniform::getClassType(void) { 
    
    static std::string revType = "Dist_logUniform";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_logUniform::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_logUniform::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        distMemberRules.push_back( new ArgumentRule( "min", RealPos::getClassTypeSpec(), "The lower bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        distMemberRules.push_back( new ArgumentRule( "max", RealPos::getClassTypeSpec(), "The upper bound.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, NULL ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_logUniform::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_logUniform::printValue(std::ostream& o) const {
    
    o << " logUniform(min=";
    if ( min != NULL ) 
    {
        o << min->getName();
    } 
    else 
    {
        o << "?";
    }
    
    o << ", max=";
    if ( max != NULL ) 
    {
        o << max->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_logUniform::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "max" )
    {
        max = var;
    }
    else if ( name == "min" )
    {
        min = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
