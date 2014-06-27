//
//  OneOverXDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "OneOverXDistribution.h"
#include "RlOneOverXDistribution.h"
#include "Real.h"
#include "RealPos.h"
#include "ContinuousStochasticNode.h"

using namespace RevLanguage;

OneOverXDistribution::OneOverXDistribution() : PositiveContinuousDistribution() {
    
}


OneOverXDistribution::~OneOverXDistribution() {
    
}



OneOverXDistribution* OneOverXDistribution::clone( void ) const {
    return new OneOverXDistribution(*this);
}


RevBayesCore::OneOverXDistribution* OneOverXDistribution::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* mi   = static_cast<const Real &>( min->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* ma   = static_cast<const Real &>( max->getRevObject() ).getDagNode();
    RevBayesCore::OneOverXDistribution* d    = new RevBayesCore::OneOverXDistribution( mi, ma );
    
    return d;
}



/* Get class name of object */
const std::string& OneOverXDistribution::getClassName(void) { 
    
    static std::string rbClassName = "1/x distribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& OneOverXDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& OneOverXDistribution::getMemberRules(void) const {
    
    static MemberRules distOneOverXMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distOneOverXMemberRules.push_back( new ArgumentRule( "min", true, RealPos::getClassTypeSpec()   , new Real(1.0E-10) ) );
        distOneOverXMemberRules.push_back( new ArgumentRule( "max", true, RealPos::getClassTypeSpec()   , new Real(1.0E4) ) );
        
        rulesSet = true;
    }
    
    return distOneOverXMemberRules;
}


const TypeSpec& OneOverXDistribution::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void OneOverXDistribution::printValue(std::ostream& o) const {
    
    o << " OneOverX(min=";
    if ( min != NULL ) 
    {
        o << min->getName();
    } 
    else 
    {
        o << "?";
    }
    
    o << ", max=";
    if ( max != NULL ) 
    {
        o << max->getName();
    } 
    else 
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void OneOverXDistribution::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
    if ( name == "max" )
    {
        max = var;
    }
    else if ( name == "min" )
    {
        min = var;
    }
    else 
    {
        Distribution::setConstMemberVariable(name, var);
    }
}
