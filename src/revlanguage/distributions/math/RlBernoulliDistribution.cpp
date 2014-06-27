//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BernoulliDistribution.h"
#include "RlBernoulliDistribution.h"
#include "RealPos.h"
#include "Probability.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

BernoulliDistribution::BernoulliDistribution() : TypedDistribution<Natural>() {
    
}


BernoulliDistribution::~BernoulliDistribution() {
    
}



BernoulliDistribution* BernoulliDistribution::clone( void ) const {
    
    return new BernoulliDistribution(*this);
}


RevBayesCore::BernoulliDistribution* BernoulliDistribution::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* q   = static_cast<const Probability &>( p->getRevObject() ).getDagNode();
    RevBayesCore::BernoulliDistribution* d  = new RevBayesCore::BernoulliDistribution( q );
    
    return d;
}



/* Get class name of object */
const std::string& BernoulliDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Bernoulli distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& BernoulliDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& BernoulliDistribution::getMemberRules(void) const {
    
    static MemberRules distBernMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distBernMemberRules.push_back( new ArgumentRule( "p", true, Probability::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distBernMemberRules;
}


const TypeSpec& BernoulliDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void BernoulliDistribution::printValue(std::ostream& o) const {
    
    o << "Bernoulli(p=";
    if ( p != NULL ) {
        o << p->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void BernoulliDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
        
    if ( name == "p" ) 
    {
        p = var;
    }
    else
    {
        TypedDistribution<Natural>::setConstMemberVariable(name, var);
    }
}
