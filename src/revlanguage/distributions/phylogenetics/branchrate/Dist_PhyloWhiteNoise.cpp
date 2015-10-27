#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_PhyloWhiteNoise.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Tree.h"
#include "PhyloWhiteNoiseProcess.h"

using namespace RevLanguage;

Dist_PhyloWhiteNoise::Dist_PhyloWhiteNoise() : TypedDistribution< ModelVector<RealPos> >()
{
    
}


Dist_PhyloWhiteNoise::~Dist_PhyloWhiteNoise()
{
    
}



Dist_PhyloWhiteNoise* Dist_PhyloWhiteNoise::clone( void ) const
{
    return new Dist_PhyloWhiteNoise(*this);
}


RevBayesCore::PhyloWhiteNoiseProcess* Dist_PhyloWhiteNoise::createDistribution( void ) const
{
    // get the parameters

    RevBayesCore::TypedDagNode<RevBayesCore::Tree>* tau = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    
    RevBayesCore::TypedDagNode<double>* s  = static_cast<const RealPos &>( sigma->getRevObject() ).getDagNode();
    
    RevBayesCore::PhyloWhiteNoiseProcess* d    = new RevBayesCore::PhyloWhiteNoiseProcess( tau, s );
    
    return d;

}



/* Get Rev type of object */
const std::string& Dist_PhyloWhiteNoise::getClassType(void)
{
    
    static std::string revType = "Dist_PhyloWhiteNoise";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_PhyloWhiteNoise::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Dist_PhyloWhiteNoise::getParameterRules(void) const
{
    
    static MemberRules dist;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        dist.push_back( new ArgumentRule( "tree" , TimeTree::getClassTypeSpec(), "The tree along which the process evolves.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        dist.push_back( new ArgumentRule( "sigma", RealPos::getClassTypeSpec() , "The standard deviation.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        rulesSet = true;
    }
    
    return dist;
}


const TypeSpec& Dist_PhyloWhiteNoise::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}




/** Print value for user */

 void Dist_PhyloWhiteNoise::printValue(std::ostream& o) const
{
    
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
void Dist_PhyloWhiteNoise::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {

    if ( name == "tree" )
    {
        tree = var;
    }
    else if ( name == "sigma" )
    {
        sigma = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}

