//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Clade.h"
#include "Func_tmrca.h"
#include "RlClade.h"
#include "RlTimeTree.h"
#include "RateMatrix.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TmrcaStatistic.h"
#include "Topology.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_tmrca::Func_tmrca( void ) : Function( ) {
    
}


/** Clone object */
Func_tmrca* Func_tmrca::clone( void ) const {
    
    return new Func_tmrca( *this );
}


RevObject* Func_tmrca::execute() {
    
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree>* tau = static_cast<const TimeTree&>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::Clade& c = static_cast<const Clade &>( this->args[1].getVariable()->getRevObject() ).getValue();
    RevBayesCore::TmrcaStatistic* f = new RevBayesCore::TmrcaStatistic( tau, c );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_tmrca::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "tree", true, TimeTree::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "clade", true, Clade::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_tmrca::getClassName(void) { 
    
    static std::string rbClassName = "Func_tmrca";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_tmrca::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_tmrca::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_tmrca::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
