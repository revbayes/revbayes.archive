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
#include "RlConstantBirthDeathProcess.h"
#include "ConstantBirthDeathProcess.h"
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

ConstantBirthDeathProcess::ConstantBirthDeathProcess() : TypedDistribution<TimeTree>() {
    
}


ConstantBirthDeathProcess::~ConstantBirthDeathProcess() {
    
}



ConstantBirthDeathProcess* ConstantBirthDeathProcess::clone( void ) const {
    return new ConstantBirthDeathProcess(*this);
}


RevBayesCore::ConstantBirthDeathProcess* ConstantBirthDeathProcess::createDistribution( void ) const {
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m = static_cast<const RealPos &>( diversification->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* s = static_cast<const RealPos &>( turnover->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<double>* r = static_cast<const Probability &>( rho->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode< std::vector<double> > *met = static_cast<const Vector<RealPos> &>( massExtinctionTimes->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode< std::vector<double> > *mep = static_cast<const Vector<RealPos> &>( massExtinctionSurvivalProbabilities->getValue() ).getValueNode();
    const std::string &strategy = static_cast<const RlString &>( samplingStrategy->getValue() ).getValue();
    const std::string& cond = static_cast<const RlString &>( condition->getValue() ).getValue();
    int n = static_cast<const Natural &>( numTaxa->getValue() ).getValue();
    const std::vector<std::string> &names = static_cast<const Vector<RlString> &>( taxonNames->getValue() ).getValueNode()->getValue();
    const std::vector<RevBayesCore::Clade> &c = static_cast<const Vector<Clade> &>( constraints->getValue() ).getValue();
    RevBayesCore::ConstantBirthDeathProcess*   d = new RevBayesCore::ConstantBirthDeathProcess(m, s, met, mep, r, strategy, cond, n, names, c);
    
    return d;
}



/* Get class name of object */
const std::string& ConstantBirthDeathProcess::getClassName(void) { 
    
    static std::string rbClassName = "constant Birth-Death Process";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& ConstantBirthDeathProcess::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Distribution::getClassTypeSpec() ) );
    
	return rbClass; 
}




/** Return member rules (no members) */
const MemberRules& ConstantBirthDeathProcess::getMemberRules(void) const {
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) {
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "diversification", true, RealPos::getClassTypeSpec() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "turnover"  , true, RealPos::getClassTypeSpec(), new RealPos(0.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "rho"  , true, Probability::getClassTypeSpec(), new Probability(1.0) ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "massExtinctionTimes"  , true, Vector<RealPos>::getClassTypeSpec(), new Vector<RealPos>() ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "massExtinctionSurvivalProbabilities"  , true, Vector<RealPos>::getClassTypeSpec(), new Vector<RealPos>() ) );
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


const TypeSpec& ConstantBirthDeathProcess::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void ConstantBirthDeathProcess::printValue(std::ostream& o) const {
    
    o << " const BDP (diversification=";
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
void ConstantBirthDeathProcess::setConstMemberVariable(const std::string& name, const RbPtr<const Variable> &var) {
    
    if ( name == "diversification" ) {
        diversification = var;
    }
    else if ( name == "turnover" ) {
        turnover = var;
    }
    else if ( name == "rho" ) {
        rho = var;
    }
    else if ( name == "samplingStrategy" ) {
        samplingStrategy = var;
    }
    else if ( name == "massExtinctionTimes" ) {
        massExtinctionTimes = var;
    }
    else if ( name == "massExtinctionSurvivalProbabilities" ) {
        massExtinctionSurvivalProbabilities = var;
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
