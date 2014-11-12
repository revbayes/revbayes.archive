#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_whiteNoise.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "TimeTree.h"
#include "WhiteNoisePhyloProcess.h"

using namespace RevLanguage;

Dist_whiteNoise::Dist_whiteNoise() : TypedDistribution< ModelVector<RealPos> >() {
    
}


Dist_whiteNoise::~Dist_whiteNoise() {
    
}



Dist_whiteNoise* Dist_whiteNoise::clone( void ) const {
    return new Dist_whiteNoise(*this);
}


RevBayesCore::WhiteNoisePhyloProcess* Dist_whiteNoise::createDistribution( void ) const {
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
    
    RevBayesCore::WhiteNoisePhyloProcess* d    = new RevBayesCore::WhiteNoisePhyloProcess( tau, s );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_whiteNoise::getClassType(void) {
    
    static std::string revType = "Dist_whiteNoise";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_whiteNoise::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_whiteNoise::getParameterRules(void) const {
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        dist.push_back( new ArgumentRule( "sigma", RealPos::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_whiteNoise::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_whiteNoise::printValue(std::ostream& o) const {
    
    o << " whitenoise(";
    
    o << "tau=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    
    o << "sigma=";
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
        o << "?";
    }
    o << ")";
}



/** Set a member variable */
void Dist_whiteNoise::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else {
        Distribution::setConstParameter(name, var);
    }
}

