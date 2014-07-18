//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_lnorm.h"
#include "LognormalDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_lnorm::Dist_lnorm() : PositiveContinuousDistribution() {
    
}


Dist_lnorm* Dist_lnorm::clone( void ) const {
    return new Dist_lnorm(*this);
}


RevBayesCore::LognormalDistribution* Dist_lnorm::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m   = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o   = static_cast<const RealPos &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::LognormalDistribution* d  = new RevBayesCore::LognormalDistribution(m, s, o);
    
    return d;
}



/* Get class name of object */
const std::string& Dist_lnorm::getClassName(void) { 
    
    static std::string rbClassName = "Dist_lnorm";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_lnorm::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_lnorm::getMemberRules(void) const {
    
    static MemberRules distLnormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distLnormMemberRules.push_back( new ArgumentRule( "mean",   true, Real::getClassTypeSpec(),    new Real(0.0) ) );
        distLnormMemberRules.push_back( new ArgumentRule( "sd"  ,   true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        distLnormMemberRules.push_back( new ArgumentRule( "offset", true, RealPos::getClassTypeSpec(), new Real(0.0) ) );
        
        rulesSet = true;
    }
    
    return distLnormMemberRules;
}


const TypeSpec& Dist_lnorm::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_lnorm::printValue(std::ostream& o) const {
    
    o << "lognormal(mean=";
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
    o << ", offset=";
    if ( offset != NULL ) {
        o << offset->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_lnorm::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "sd" ) 
    {
        sd = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else 
    {
        PositiveContinuousDistribution::setConstMemberVariable(name, var);
    }
}
