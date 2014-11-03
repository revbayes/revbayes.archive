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



/* Get Rev type of object */
const std::string& Dist_oneOverX::getClassType(void) { 
    
    static std::string revType = "Dist_oneOverX";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_oneOverX::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_oneOverX::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        distMemberRules.push_back( new ArgumentRule( "min", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distMemberRules.push_back( new ArgumentRule( "max", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distMemberRules;
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
void Dist_oneOverX::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        Distribution::setConstParameter(name, var);
    }
}
