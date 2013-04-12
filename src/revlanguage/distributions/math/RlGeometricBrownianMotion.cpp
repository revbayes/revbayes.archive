//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "GeometricBrownianMotion.h"
#include "RlGeometricBrownianMotion.h"
#include "Real.h"
#include "RealPos.h"
#include "Natural.h"
#include "StochasticNode.h"
#include "UnivariateFunction.h"

using namespace RevLanguage;

GeometricBrownianMotion::GeometricBrownianMotion() : TypedDistribution<UnivariateFunction>() {
    
}


GeometricBrownianMotion::~GeometricBrownianMotion() {
    
}



GeometricBrownianMotion* GeometricBrownianMotion::clone( void ) const {
    return new GeometricBrownianMotion(*this);
}


RevBayesCore::TypedDistribution<RevBayesCore::UnivariateFunction>* GeometricBrownianMotion::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* i   = static_cast<const RealPos &>( initial->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sigma->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* t   = static_cast<const RealPos &>( time->getValue() ).getValueNode();
    int n = static_cast<const Natural &>( nBlocks->getValue() ).getValueNode()->getValue();
    RevBayesCore::GeometricBrownianMotion* d  = new RevBayesCore::GeometricBrownianMotion(i, s, t, n);
//    RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction>* rv  = new RevBayesCore::StochasticNode<RevBayesCore::UnivariateFunction>("", d);
    
    return d;
}



/* Get class name of object */
const std::string& GeometricBrownianMotion::getClassName(void) { 
    
    static std::string rbClassName = "geometric Brownian motion";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& GeometricBrownianMotion::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& GeometricBrownianMotion::getMemberRules(void) const {
    
    static MemberRules distgeoBrownianMotionMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distgeoBrownianMotionMemberRules.push_back( new ArgumentRule( "initial", true, RealPos::getClassTypeSpec() ) );
        distgeoBrownianMotionMemberRules.push_back( new ArgumentRule( "sigma"  , true, RealPos::getClassTypeSpec() ) );
        distgeoBrownianMotionMemberRules.push_back( new ArgumentRule( "time"  , true, RealPos::getClassTypeSpec() ) );
        distgeoBrownianMotionMemberRules.push_back( new ArgumentRule( "nBlocks"  , true, Natural::getClassTypeSpec(), new Natural(10) ) );
        
        rulesSet = true;
    }
    
    return distgeoBrownianMotionMemberRules;
}


const TypeSpec& GeometricBrownianMotion::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void GeometricBrownianMotion::printValue(std::ostream& o) const {
    
    o << "geometric Brownian motion(initial=";
    if ( initial != NULL ) {
        o << initial->getName();
    } else {
        o << "?";
    }
    o << ", sigma=";
    if ( sigma != NULL ) {
        o << sigma->getName();
    } else {
        o << "?";
    }
    o << ", time=";
    if ( time != NULL ) {
        o << time->getName();
    } else {
        o << "?";
    }
    o << ", nBlocks=";
    if ( nBlocks != NULL ) {
        o << nBlocks->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void GeometricBrownianMotion::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "initial" ) {
        initial = var;
    }
    else if ( name == "sigma" ) {
        sigma = var;
    }
    else if ( name == "time" ) {
        time = var;
    }
    else if ( name == "nBlocks" ) {
        nBlocks = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
