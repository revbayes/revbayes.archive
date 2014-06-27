//
//  ExponentialDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_mvtnorm.h"
#include "MultivariateNormalDistribution.h"
#include "Real.h"
#include "StochasticNode.h"
#include "Vector.h"
#include "PrecisionMatrix.h"
#include "RealSymmetricMatrix.h"

using namespace RevLanguage;

Dist_mvtnorm::Dist_mvtnorm() : TypedDistribution<Vector<Real> >() {
    
}


Dist_mvtnorm::~Dist_mvtnorm() {
    
}



Dist_mvtnorm* Dist_mvtnorm::clone( void ) const {
    return new Dist_mvtnorm(*this);
}


RevBayesCore::MultivariateNormalDistribution* Dist_mvtnorm::createDistribution( void ) const {

    // get the parameters
    RevBayesCore::TypedDagNode<std::vector<double> >* m = static_cast<const Vector<Real> &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix >* p = static_cast<const RealSymmetricMatrix &>( precision->getRevObject() ).getDagNode();
    RevBayesCore::MultivariateNormalDistribution* d     = new RevBayesCore::MultivariateNormalDistribution( m,p );
    
    return d;
}



/* Get class name of object */
const std::string& Dist_mvtnorm::getClassName(void) {
    
    static std::string rbClassName = "Dist_mvtnorm";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_mvtnorm::getClassTypeSpec(void) {
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_mvtnorm::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "mean", true, Vector<Real>::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "precision", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
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
void Dist_mvtnorm::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        Distribution::setConstMemberVariable(name, var);
    }
}
