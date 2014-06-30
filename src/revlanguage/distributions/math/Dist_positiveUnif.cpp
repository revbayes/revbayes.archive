//
//  NormalDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ContinuousStochasticNode.h"
#include "Dist_positiveUnif.h"
#include "Real.h"
#include "RealPos.h"
#include "UniformDistribution.h"

using namespace RevLanguage;

Dist_positiveUnif::Dist_positiveUnif() : PositiveContinuousDistribution() {
    
}


Dist_positiveUnif::~Dist_positiveUnif() {
    
}



Dist_positiveUnif* Dist_positiveUnif::clone( void ) const {
    return new Dist_positiveUnif(*this);
}


RevBayesCore::UniformDistribution* Dist_positiveUnif::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* l   = static_cast<const RealPos &>( lower->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* u   = static_cast<const RealPos &>( upper->getRevObject() ).getDagNode();
    RevBayesCore::UniformDistribution* d    = new RevBayesCore::UniformDistribution(l, u);
    
    return d;
}



/* Get class name of object */
const std::string& Dist_positiveUnif::getClassName(void) { 
    
    static std::string rbClassName = "Dist_unif";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_positiveUnif::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& Dist_positiveUnif::getMemberRules(void) const {
    
    static MemberRules distUnifMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distUnifMemberRules.push_back( new ArgumentRule( "lower", true, RealPos::getClassTypeSpec()   , new Real(0.0) ) );
        distUnifMemberRules.push_back( new ArgumentRule( "upper"  , true, RealPos::getClassTypeSpec(), new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return distUnifMemberRules;
}


const TypeSpec& Dist_positiveUnif::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_positiveUnif::printValue(std::ostream& o) const {
    
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
void Dist_positiveUnif::setConstMemberVariable(const std::string& name, const RevPtr<const Variable> &var) {
    
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
        Distribution::setConstMemberVariable(name, var);
    }
}
