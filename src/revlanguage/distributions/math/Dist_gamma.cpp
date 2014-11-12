#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_gamma.h"
#include "GammaDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_gamma::Dist_gamma() : PositiveContinuousDistribution() {
    
}


Dist_gamma::~Dist_gamma() {
    
}



Dist_gamma* Dist_gamma::clone( void ) const {
    
    return new Dist_gamma(*this);
}


RevBayesCore::GammaDistribution* Dist_gamma::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* sh  = static_cast<const RealPos &>( shape->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* r   = static_cast<const RealPos &>( rate->getRevObject() ).getDagNode();
    RevBayesCore::GammaDistribution* d      = new RevBayesCore::GammaDistribution(sh, r);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_gamma::getClassType(void) { 
    
    static std::string revType = "Dist_gamma";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_gamma::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_gamma::getParameterRules(void) const {
    
    static MemberRules distGammaMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distGammaMemberRules.push_back( new ArgumentRule( "shape", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distGammaMemberRules.push_back( new ArgumentRule( "rate" , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distGammaMemberRules;
}


const TypeSpec& Dist_gamma::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_gamma::printValue(std::ostream& o) const {
    
    o << "gamma(shape=";
    if ( shape != NULL ) 
    {
        o << shape->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ", rate=";
    if ( rate != NULL ) 
    {
        o << rate->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_gamma::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "shape" ) 
    {
        shape = var;
    }
    else if ( name == "rate" ) 
    {
        rate = var;
    }
    else  
    {
        PositiveContinuousDistribution::setConstParameter(name, var);
    }
}
