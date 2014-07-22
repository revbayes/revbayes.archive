//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "LogFunction.h"
#include "Func_log.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_log::Func_log( void ) : Function( ) {
    
}


/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


RevPtr<Variable> Func_log::execute() {
    
    RevBayesCore::TypedDagNode<double>* a = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* b = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::LogFunction* f = new RevBayesCore::LogFunction( a, b );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    Real* value = new Real( detNode );
    
    return new Variable( value );
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


const std::string& Func_log::getClassType(void) { 
    
    static std::string revType = "Func_log";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_log::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
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
