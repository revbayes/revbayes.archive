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
#include "RlUniformDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

UniformDistribution::UniformDistribution() : ContinuousDistribution() {
    
}


UniformDistribution::~UniformDistribution() {
    
}



UniformDistribution* UniformDistribution::clone( void ) const {
    return new UniformDistribution(*this);
}


RevBayesCore::UniformDistribution* UniformDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const Real &>( lower->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const Real &>( upper->getValue() ).getValueNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get class name of object */
const std::string& UniformDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Uniform distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& UniformDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& UniformDistribution::getMemberRules(void) const {
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distUnifMemberRules.push_back( new ArgumentRule( "lower", true, Real::getClassTypeSpec() ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper"  , true, Real::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& UniformDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void UniformDistribution::printValue(std::ostream& o) const {
    
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
void UniformDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
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
        ContinuousDistribution::setConstMemberVariable(name, var);
    }
}
