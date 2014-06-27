//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "GammaDistribution.h"
#include "RlGammaDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

GammaDistribution::GammaDistribution() : PositiveContinuousDistribution() {
    
}


GammaDistribution::~GammaDistribution() {
    
}



GammaDistribution* GammaDistribution::clone( void ) const {
    
    return new GammaDistribution(*this);
}


RevBayesCore::GammaDistribution* GammaDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* sh  = static_cast<const RealPos &>( shape->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* r   = static_cast<const RealPos &>( rate->getValue() ).getValueNode();
    RevBayesCore::GammaDistribution* d      = new RevBayesCore::GammaDistribution(sh, r);
    
    return d;
}



/* Get class name of object */
const std::string& GammaDistribution::getClassName(void) { 
    
    static std::string rbClassName = "GammaDistribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& GammaDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& GammaDistribution::getMemberRules(void) const {
    
    static MemberRules distGammaMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distGammaMemberRules.push_back( new ArgumentRule( "shape", true, RealPos::getClassTypeSpec() ) );
        distGammaMemberRules.push_back( new ArgumentRule( "rate" , true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return distGammaMemberRules;
}


const TypeSpec& GammaDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void GammaDistribution::printValue(std::ostream& o) const {
    
    o << "gamma(shape=";
    if ( shape != NULL ) 
    {
        o << shape->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ", rate=";
    if ( rate != NULL ) 
    {
        o << rate->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void GammaDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "shape" ) 
    {
        shape = var;
    }
    else if ( name == "rate" ) 
    {
        rate = var;
    }
    else  
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
