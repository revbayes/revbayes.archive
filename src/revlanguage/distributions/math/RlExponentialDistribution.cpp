//
//  ExponentialDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ExponentialDistribution.h"
#include "RlExponentialDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

ExponentialDistribution::ExponentialDistribution() : PositiveContinuousDistribution() {
    
}


ExponentialDistribution::~ExponentialDistribution() {
    
}



ExponentialDistribution* ExponentialDistribution::clone( void ) const {
    return new ExponentialDistribution(*this);
}


RevBayesCore::ExponentialDistribution* ExponentialDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l       = static_cast<const Real &>( lambda->getValue() ).getValueNode();
    RevBayesCore::ExponentialDistribution* d    = new RevBayesCore::ExponentialDistribution( l );
    
    return d;
}



/* Get class name of object */
const std::string& ExponentialDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Exponential distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& ExponentialDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& ExponentialDistribution::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec()   , new Real(1.0) ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& ExponentialDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void ExponentialDistribution::printValue(std::ostream& o) const {
    
    o << " exponential(lambda=";
    if ( lambda != NULL ) {
        o << lambda->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void ExponentialDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "lambda" ) {
        lambda = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
