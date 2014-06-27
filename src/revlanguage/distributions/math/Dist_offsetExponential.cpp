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
#include "RlOffsetExponentialDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

OffsetExponentialDistribution::OffsetExponentialDistribution() : ContinuousDistribution() {
    
}


OffsetExponentialDistribution::~OffsetExponentialDistribution() {
    
}



OffsetExponentialDistribution* OffsetExponentialDistribution::clone( void ) const {
    return new OffsetExponentialDistribution(*this);
}


RevBayesCore::ExponentialDistribution* OffsetExponentialDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l     = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o     = static_cast<const Real    &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialDistribution* d  = new RevBayesCore::ExponentialDistribution( l, o );
    
    return d;
}



/* Get class name of object */
const std::string& OffsetExponentialDistribution::getClassName(void) { 
    
    static std::string rbClassName = "Dist_offsetExponential";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& OffsetExponentialDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& OffsetExponentialDistribution::getMemberRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec()   , new RealPos(1.0) ) );
        distExpMemberRules.push_back( new ArgumentRule( "offset", true, Real::getClassTypeSpec()  ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& OffsetExponentialDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void OffsetExponentialDistribution::printValue(std::ostream& o) const {
    
    o << " exponential(lambda=";
    if ( lambda != NULL ) {
        o << lambda->getName();
    } else {
        o << "?";
    }
    o << ", offset=";
    if ( offset != NULL ) {
        o << offset->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void OffsetExponentialDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "offset" ) 
    {
        offset = var;
    }
    else 
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
