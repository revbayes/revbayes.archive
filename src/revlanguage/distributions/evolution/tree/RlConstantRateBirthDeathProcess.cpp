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
#include "RlConstantRateBirthDeathProcess.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "Vector.h"

using namespace RevLanguage;

ConstantRateBirthDeathProcess::ConstantRateBirthDeathProcess() : TypedDistribution<TimeTree>() {
    
}


ConstantRateBirthDeathProcess* ConstantRateBirthDeathProcess::clone( void ) const {
    return new ConstantRateBirthDeathProcess(*this);
}


RevBayesCore::ConstantRateBirthDeathProcess* ConstantRateBirthDeathProcess::createDistribution( void ) const {
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* o = static_cast<const RealPos &>( origin->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* s = static_cast<const RealPos &>( lambda->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* e = static_cast<const RealPos &>( mu->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* r = static_cast<const Probability &>( rho->getValue() ).getValueNode();
    const std::string &strategy = static_cast<const RlString &>( samplingStrategy->getValue() ).getValue();
    const std::string& cond = static_cast<const RlString &>( condition->getValue() ).getValue();
    int n = static_cast<const Natural &>( numTaxa->getValue() ).getValue();
    const std::vector<std::string> &names = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    const std::vector<RevBayesCore::Clade> &c = static_cast<const Vector<Clade> &>( constraints->getValue() ).getValue();
    RevBayesCore::ConstantRateBirthDeathProcess*   d = new RevBayesCore::ConstantRateBirthDeathProcess(o, s, e, r, strategy, cond, n, names, c);
    
    return d;
}



/* Get class name of object */
const std::string& ConstantRateBirthDeathProcess::getClassName(void) { 
    
    static std::string rbClassName = "constant-rate Birth-Death Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& ConstantRateBirthDeathProcess::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& ConstantRateBirthDeathProcess::getMemberRules(void) const {
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "origin", true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda", true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "mu"  , true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"  , true, Probability::getClassTypeSpec(), new Probability(1.0) ) );
        Vector<RlString> optionsStrategy;
        optionsStrategy.push_back( RlString("uniform") );
        optionsStrategy.push_back( RlString("diversified") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "samplingStrategy", new RlString("uniform"), optionsStrategy ) );
        Vector<RlString> optionsCondition;
        optionsCondition.push_back( RlString("time") );
        optionsCondition.push_back( RlString("survival") );
        distcBirthDeathMemberRules.push_back( new OptionRule( "condition", new RlString("time"), optionsCondition ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "nTaxa"  , true, Natural::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "names"  , true, Vector<RlString>::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "constraints"  , true, Vector<Clade>::getClassTypeSpec(), new Vector<Clade>() ) );
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
}


const TypeSpec& ConstantRateBirthDeathProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void ConstantRateBirthDeathProcess::printValue(std::ostream& o) const {
    
    o << " const BDP (lambda=";
    if ( lambda != NULL ) {
        o << lambda->getName();
    } else {
        o << "?";
    }
    o << ", mu=";
    if ( mu != NULL ) {
        o << mu->getName();
    } else {
        o << "?";
    }
    o << ", rho=";
    if ( rho != NULL ) {
        o << rho->getName();
    } else {
        o << "?";
    }
    o << ", samplingStrategy=";
    if ( samplingStrategy != NULL ) {
        o << samplingStrategy->getName();
    } else {
        o << "?";
    }
    o << ", condition=";
    if ( condition != NULL ) {
        o << condition->getName();
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
void ConstantRateBirthDeathProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "origin" ) 
    {
        origin = var;
    }
    else if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "mu" ) 
    {
        mu = var;
    }
    else if ( name == "rho" ) 
    {
        rho = var;
    }
    else if ( name == "samplingStrategy" ) 
    {
        samplingStrategy = var;
    }
    else if ( name == "nTaxa" ) 
    {
        numTaxa = var;
    }
    else if ( name == "names" ) 
    {
        taxonNames = var;
    }
    else if ( name == "constraints" ) 
    {
        constraints = var;
    }
    else if ( name == "condition" ) 
    {
        condition = var;
    }
    else {
        Distribution::setConstMemberVariable(name, var);
    }
}
