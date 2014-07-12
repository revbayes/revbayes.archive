//
//  Dist_oneOverX.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_oneOverX.h"
#include "OneOverXDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_oneOverX::Dist_oneOverX() : PositiveContinuousDistribution() {
    
}


Dist_oneOverX::~Dist_oneOverX() {
    
}



Dist_oneOverX* Dist_oneOverX::clone( void ) const {
    return new Dist_oneOverX(*this);
}


RevBayesCore::OneOverXDistribution* Dist_oneOverX::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* mi   = static_cast<const Real &>( min->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* ma   = static_cast<const Real &>( max->getRevObject() ).getDagNode();
    RevBayesCore::OneOverXDistribution* d    = new RevBayesCore::OneOverXDistribution( mi, ma );
    
    return d;
}



/* Get class name of object */
const std::string& Dist_oneOverX::getClassType(void) { 
    
    static std::string rbClassName = "Dist_oneOverX";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_oneOverX::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_oneOverX::getMemberRules(void) const {
    
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


const TypeSpec& Dist_oneOverX::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_oneOverX::printValue(std::ostream& o) const {
    
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
void Dist_oneOverX::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
