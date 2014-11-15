#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_mvtnorm.h"
#include "ModelVector.h"
#include "MultivariateNormalDistribution.h"
#include "Real.h"
#include "StochasticNode.h"
#include "MatrixRealSymmetric.h"
#include "RealSymmetricMatrix.h"

using namespace RevLanguage;

Dist_mvtnorm::Dist_mvtnorm() : TypedDistribution<ModelVector<Real> >() {
    
}


Dist_mvtnorm::~Dist_mvtnorm() {
    
}



Dist_mvtnorm* Dist_mvtnorm::clone( void ) const {
    return new Dist_mvtnorm(*this);
}


RevBayesCore::MultivariateNormalDistribution* Dist_mvtnorm::createDistribution( void ) const {

    // get the parameters
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* m = static_cast<const ModelVector<Real> &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::MatrixRealSymmetric >* p = static_cast<const RealSymmetricMatrix &>( precision->getRevObject() ).getDagNode();
    RevBayesCore::MultivariateNormalDistribution* d     = new RevBayesCore::MultivariateNormalDistribution( m,p );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_mvtnorm::getClassType(void) {
    
    static std::string revType = "Dist_mvtnorm";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_mvtnorm::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_mvtnorm::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        distMemberRules.push_back( new ArgumentRule( "mean"     , ModelVector<Real>::getClassTypeSpec()  , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "precision", RealSymmetricMatrix::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
}


const TypeSpec& Dist_mvtnorm::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_mvtnorm::printValue(std::ostream& o) const {
    
    o << " MultivariateNormal(mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ",";
    if ( precision != NULL ) {
        o << precision->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_mvtnorm::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "precision" ) 
    {
        precision = var;
    }
    else 
    {
        Distribution::setConstParameter(name, var);
    }
}
