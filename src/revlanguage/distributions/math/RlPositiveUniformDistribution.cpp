//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "UniformDistribution.h"
#include "RlPositiveUniformDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

PositiveUniformDistribution::PositiveUniformDistribution() : PositiveContinuousDistribution() {
    
}


PositiveUniformDistribution::~PositiveUniformDistribution() {
    
}



PositiveUniformDistribution* PositiveUniformDistribution::clone( void ) const {
    return new PositiveUniformDistribution(*this);
}


RevBayesCore::UniformDistribution* PositiveUniformDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const RealPos &>( lower->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const RealPos &>( upper->getValue() ).getValueNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get class name of object */
const std::string& PositiveUniformDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Uniform distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& PositiveUniformDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& PositiveUniformDistribution::getMemberRules(void) const {
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distUnifMemberRules.push_back( new ArgumentRule( "lower", true, RealPos::getClassTypeSpec()   , new Real(0.0) ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper"  , true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& PositiveUniformDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void PositiveUniformDistribution::printValue(std::ostream& o) const {
    
    o << " unif (lower=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ", upper=";
    if ( lower != NULL ) {
        o << lower->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void PositiveUniformDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "lower" ) 
    {
        lower = var;
    }
    else if ( name == "upper" ) 
    {
        upper = var;
    }
    else 
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
