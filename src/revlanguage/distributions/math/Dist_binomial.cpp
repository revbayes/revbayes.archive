//
//  Dist_binomial
//  RevBayesCore
//
//  Created by John Huelsenbeck on 8/25/14.
//  Copyright 2012 UC Berkeley. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "BinomialDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_binomial.h"
#include "Probability.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_binomial::Dist_binomial(void) : TypedDistribution<Natural>() {
    
}


Dist_binomial::~Dist_binomial(void) {
    
}



Dist_binomial* Dist_binomial::clone( void ) const {
    
    return new Dist_binomial(*this);
}


RevBayesCore::BinomialDistribution* Dist_binomial::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<int>*    vn = static_cast<const Natural     &>( n->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* vp = static_cast<const Probability &>( p->getRevObject() ).getDagNode();
    RevBayesCore::BinomialDistribution* d  = new RevBayesCore::BinomialDistribution( vn, vp );
    return d;
}



/* Get Rev type of object */
const std::string& Dist_binomial::getClassType(void) {
    
    static std::string revType = "Dist_binomial";
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_binomial::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Distribution::getClassTypeSpec() ) );
	return revTypeSpec;
}




/** Return member rules (no members) */
const MemberRules& Dist_binomial::getParameterRules(void) const {
    
    static MemberRules distBinMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distBinMemberRules.push_back( new ArgumentRule( "p", Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distBinMemberRules.push_back( new ArgumentRule( "n", Natural::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return distBinMemberRules;
}


const TypeSpec& Dist_binomial::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    return ts;
}


/** Print value for user */
void Dist_binomial::printValue(std::ostream& o) const {
    
    o << "Binomial(p=";
    if ( p != NULL )
        o << p->getName();
    else
        o << "?";
    o << ", n=";
    if ( n != NULL )
        o << n->getName();
    else
        o << "?";
    o << ")";
}


/** Set a member variable */
void Dist_binomial::setConstParameter(const std::string& name, const RevPtr<const Variable> &var) {
        
    if ( name == "p" ) 
        {
        p = var;
        }
    else if ( name == "n" )
        {
        n = var;
        }
    else
        {
        TypedDistribution<Natural>::setConstParameter(name, var);
        }
}
