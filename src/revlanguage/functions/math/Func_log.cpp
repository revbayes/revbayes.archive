//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "LogFunction.h"
#include "Func_log.h"
#include "Real.h"
#include "RealPos.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_log::Func_log( void ) : Function( ) {
    
}


/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


RevObject* Func_log::execute() {
    
    RevBayesCore::TypedDagNode<double>* a = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* b = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::LogFunction* f = new RevBayesCore::LogFunction( a, b );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    Real* value = new Real( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_log::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "base", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_log::getClassName(void) { 
    
    static std::string rbClassName = "Func_log";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_log::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_log::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_log::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
