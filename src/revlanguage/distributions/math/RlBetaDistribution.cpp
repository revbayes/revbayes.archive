//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BetaDistribution.h"
#include "RlBetaDistribution.h"
#include "RealPos.h"
#include "Probability.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

BetaDistribution::BetaDistribution() : TypedDistribution<Probability>() {
    
}


BetaDistribution::~BetaDistribution() {
    
}



BetaDistribution* BetaDistribution::clone( void ) const {
    
    return new BetaDistribution(*this);
}


RevBayesCore::BetaDistribution* BetaDistribution::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* a   = static_cast<const RealPos &>( alpha->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* b   = static_cast<const RealPos &>( beta->getValue() ).getValueNode();
    RevBayesCore::BetaDistribution* d       = new RevBayesCore::BetaDistribution(a, b);
    
    return d;
}



Probability* BetaDistribution::createRandomVariable(void) const { 
    
    RevBayesCore::ContinuousDistribution* d = createDistribution();
    RevBayesCore::TypedDagNode<double>* rv  = new RevBayesCore::ContinuousStochasticNode("", d);
    
    return new Probability(rv);
}



/* Get class name of object */
const std::string& BetaDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Beta distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& BetaDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& BetaDistribution::getMemberRules(void) const {
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distUnifMemberRules.push_back( new ArgumentRule( "alpha", true, RealPos::getClassTypeSpec() ) );
        distUnifMemberRules.push_back( new ArgumentRule( "beta"  , true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& BetaDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void BetaDistribution::printValue(std::ostream& o) const {
    
    o << "beta(alpha=";
    if ( alpha != NULL ) {
        o << alpha->getName();
    } else {
        o << "?";
    }
    o << ", beta=";
    if ( beta != NULL ) {
        o << beta->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void BetaDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
        
    if ( name == "alpha" ) 
    {
        alpha = var;
    }
    else if ( name == "beta" ) 
    {
        beta = var;
    }
    else
    {
        TypedDistribution<Probability>::setConstMemberVariable(name, var);
    }
}
