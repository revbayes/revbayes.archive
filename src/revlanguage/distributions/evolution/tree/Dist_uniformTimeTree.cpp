#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUniformTimeTreeDistribution.h"
#include "StochasticNode.h"
#include "UniformTimeTreeDistribution.h"
#include "Vector.h"

using namespace RevLanguage;


UniformTimeTreeDistribution::UniformTimeTreeDistribution() : TypedDistribution<TimeTree>() {
    
}


UniformTimeTreeDistribution::~UniformTimeTreeDistribution() {

}



UniformTimeTreeDistribution* UniformTimeTreeDistribution::clone( void ) const {

    return new UniformTimeTreeDistribution( *this );
}


RevBayesCore::UniformTimeTreeDistribution* UniformTimeTreeDistribution::createDistribution( void ) const {

    // Get the parameters
    RevBayesCore::TypedDagNode<double>* originT = static_cast<const RealPos &>( originTime->getRevObject() ).getDagNode();
    const std::vector<std::string>      &names  = static_cast<const Vector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();

    RevBayesCore::UniformTimeTreeDistribution*   d = new RevBayesCore::UniformTimeTreeDistribution( originT, names );

    return d;
}



/* Get class name of object */
const std::string& UniformTimeTreeDistribution::getClassName(void) {
    
    static std::string rbClassName = "Dist_uniformTimeTree";
    
	return rbClassName;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& UniformTimeTreeDistribution::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
	return rbClass;
}


/* Return member rules */
const MemberRules& UniformTimeTreeDistribution::getMemberRules(void) const {
    
    static MemberRules distUTTDMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {

        distUTTDMemberRules.push_back( new ArgumentRule( "originTime", true, RealPos::getClassTypeSpec() ) );
        distUTTDMemberRules.push_back( new ArgumentRule( "taxonNames", true, Vector<RlString>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distUTTDMemberRules;
}


const TypeSpec& UniformTimeTreeDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void UniformTimeTreeDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "originTime" ) {
        originTime = var;
    }
    else if ( name == "taxonNames" ) {
        taxonNames = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}

