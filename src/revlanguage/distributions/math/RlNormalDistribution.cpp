//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlNormalDistribution.h"
#include "NormalDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

NormalDistribution::NormalDistribution() : ContinuousDistribution() {
    
}


NormalDistribution::~NormalDistribution() {
    
}



NormalDistribution* NormalDistribution::clone( void ) const {
    return new NormalDistribution(*this);
}


RevBayesCore::NormalDistribution* NormalDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m = static_cast<const Real &>( mean->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* s = static_cast<const RealPos &>( sd->getValue() ).getValueNode();
    RevBayesCore::NormalDistribution*   d = new RevBayesCore::NormalDistribution(m, s);
    
    return d;
}



/* Get class name of object */
const std::string& NormalDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Normal distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& NormalDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& NormalDistribution::getMemberRules(void) const {
    
    static MemberRules distNormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distNormMemberRules.push_back( new ArgumentRule( "mean", true, Real::getClassTypeSpec()   , new Real(0.0) ) );
        distNormMemberRules.push_back( new ArgumentRule( "sd"  , true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
    
        rulesSet = true;
    }
    
    return distNormMemberRules;
}


const TypeSpec& NormalDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void NormalDistribution::printValue(std::ostream& o) const {
    
    o << " norm (mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL ) {
        o << sd->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void NormalDistribution::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "mean" ) {
        mean = var;
    }
    else if ( name == "sd" ) {
        sd = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
