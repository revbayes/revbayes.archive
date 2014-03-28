//
//  ExponentialDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MultivariateNormalDistribution.h"
#include "RlMultivariateNormalDistribution.h"
#include "Real.h"
#include "StochasticNode.h"
#include "Vector.h"
#include "PrecisionMatrix.h"
#include "RealSymmetricMatrix.h"

using namespace RevLanguage;

MultivariateNormalDistribution::MultivariateNormalDistribution() : TypedDistribution<Vector<Real> >() {
    
}


MultivariateNormalDistribution::~MultivariateNormalDistribution() {
    
}



MultivariateNormalDistribution* MultivariateNormalDistribution::clone( void ) const {
    return new MultivariateNormalDistribution(*this);
}


RevBayesCore::MultivariateNormalDistribution* MultivariateNormalDistribution::createDistribution( void ) const {

    // get the parameters
    RevBayesCore::TypedDagNode<std::vector<double> >* m = static_cast<const Vector<Real> &>( mean->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<RevBayesCore::PrecisionMatrix >* p = static_cast<const RealSymmetricMatrix &>( precision->getValue() ).getValueNode();
    RevBayesCore::MultivariateNormalDistribution* d    = new RevBayesCore::MultivariateNormalDistribution( m,p );
    
    return d;
}



/* Get class name of object */
const std::string& MultivariateNormalDistribution::getClassName(void) { 
    
    static std::string rbClassName = "MultivariateNormal distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& MultivariateNormalDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& MultivariateNormalDistribution::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "mean", true, Vector<Real>::getClassTypeSpec() ) );
        distExpMemberRules.push_back( new ArgumentRule( "precision", true, RealSymmetricMatrix::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& MultivariateNormalDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void MultivariateNormalDistribution::printValue(std::ostream& o) const {
    
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
void MultivariateNormalDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "mean" ) {
        mean = var;
    }
    else if ( name == "precision" ) {
        precision = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
