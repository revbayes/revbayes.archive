//
//  ConstantBirthDeathProcess.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/6/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "RlTimeDependentBirthDeathProcess.h"
#include "TimeDependentBirthDeathProcess.h"
#include "Natural.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlUnivariateFunction.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

TimeDependentBirthDeathProcess::TimeDependentBirthDeathProcess() : TypedDistribution<TimeTree>() {
    
}


TimeDependentBirthDeathProcess::~TimeDependentBirthDeathProcess() {
    
}



TimeDependentBirthDeathProcess* TimeDependentBirthDeathProcess::clone( void ) const {
    return new TimeDependentBirthDeathProcess(*this);
}


RevBayesCore::TimeDependentBirthDeathProcess* TimeDependentBirthDeathProcess::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction>* m = static_cast<RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction>*>( static_cast<const UnivariateFunction &>( diversification->getValue() ).getValueNode() );
    RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction>* s = static_cast<RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction>*>( static_cast<const UnivariateFunction &>( turnover->getValue() ).getValueNode() );
    RevBayesCore::TypedDagNode<double>* r = static_cast<const Probability &>( rho->getValue() ).getValueNode();
    int n = static_cast<const Natural &>( numTaxa->getValue() ).getValueNode()->getValue();
    const std::vector<std::string> &names = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    const std::vector<RevBayesCore::Clade> &c = static_cast<const Vector<Clade> &>( constraints->getValue() ).getValue();
    RevBayesCore::TimeDependentBirthDeathProcess*   d = new RevBayesCore::TimeDependentBirthDeathProcess(m, s, r, n, names, c);
    
    return d;
}



/* Get class name of object */
const std::string& TimeDependentBirthDeathProcess::getClassName(void) { 
    
    static std::string rbClassName = "time-dependent Birth-Death Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& TimeDependentBirthDeathProcess::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& TimeDependentBirthDeathProcess::getMemberRules(void) const {
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "diversification", true, UnivariateFunction::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "turnover"  , true, UnivariateFunction::getClassTypeSpec(), new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"  , true, Probability::getClassTypeSpec(), new Probability(1.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"  , true, Vector<Clade>::getClassTypeSpec(), new Vector<Clade>() ) );
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
}


const TypeSpec& TimeDependentBirthDeathProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void TimeDependentBirthDeathProcess::printValue(std::ostream& o) const {
    
    o << " time-dependent BDP (diversification=";
    if ( diversification != NULL ) {
        o << diversification->getName();
    } else {
        o << "?";
    }
    o << ", turnover=";
    if ( turnover != NULL ) {
        o << turnover->getName();
    } else {
        o << "?";
    }
    o << ", rho=";
    if ( rho != NULL ) {
        o << rho->getName();
    } else {
        o << "?";
    }
    o << ", nTaxa=";
    if ( numTaxa != NULL ) {
        o << numTaxa->getName();
    } else {
        o << "?";
    }
    o << ", names=";
    if ( taxonNames != NULL ) {
        o << taxonNames->getName();
    } else {
        o << "?";
    }
    o << ", constraints=";
    if ( constraints != NULL ) {
        o << constraints->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void TimeDependentBirthDeathProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "diversification" ) {
        diversification = var;
    }
    else if ( name == "turnover" ) {
        turnover = var;
    }
    else if ( name == "rho" ) {
        rho = var;
    }
    else if ( name == "nTaxa" ) {
        numTaxa = var;
    }
    else if ( name == "names" ) {
        taxonNames = var;
    }
    else if ( name == "constraints" ) {
        constraints = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
