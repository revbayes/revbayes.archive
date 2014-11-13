//
//  ExponentialDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_offsetExponential.h"
#include "ExponentialDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

Dist_offsetExponential::Dist_offsetExponential() : ContinuousDistribution() {
    
}


Dist_offsetExponential::~Dist_offsetExponential() {
    
}



Dist_offsetExponential* Dist_offsetExponential::clone( void ) const {
    return new Dist_offsetExponential(*this);
}


RevBayesCore::ExponentialDistribution* Dist_offsetExponential::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l     = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o     = static_cast<const Real    &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialDistribution* d  = new RevBayesCore::ExponentialDistribution( l, o );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_offsetExponential::getClassType(void) { 
    
    static std::string revType = "Dist_offsetExponential";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_offsetExponential::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_offsetExponential::getParameterRules(void) const {
    
    static MemberRules distExpMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distExpMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distExpMemberRules.push_back( new ArgumentRule( "offset", Real::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distExpMemberRules;
}


const TypeSpec& Dist_offsetExponential::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_offsetExponential::printValue(std::ostream& o) const {
    
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
void Dist_offsetExponential::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
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
        Distribution::setConstParameter(name, var);
    }
}
