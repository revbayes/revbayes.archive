//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "DeterministicNode.h"
#include "TmrcaStatistic.h"
#include "RlClade.h"
#include "RlTmrcaStatistic.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
TmrcaStatistic::TmrcaStatistic( void ) : Function( ) {
    
}


/** Clone object */
TmrcaStatistic* TmrcaStatistic::clone( void ) const {
    
    return new TmrcaStatistic( *this );
}


RbLanguageObject* TmrcaStatistic::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getValue() ).getValueNode();
    const RevBayesCore::Clade& c = static_cast<const Clade &>( this->args[1].getVariable()->getValue() ).getValue();
    RevBayesCore::TmrcaStatistic* f = new RevBayesCore::TmrcaStatistic( tau, c );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    RealPos* value = new RealPos( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& TmrcaStatistic::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "clade", true, Clade::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& TmrcaStatistic::getClassName(void) { 
    
    static std::string rbClassName = "TMRCA-Statistic";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& TmrcaStatistic::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& TmrcaStatistic::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& TmrcaStatistic::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
