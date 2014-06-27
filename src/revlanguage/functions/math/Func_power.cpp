//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "PowerFunction.h"
#include "Func_power.h"
#include "Real.h"
#include "RealPos.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_power::Func_power( void ) : Function( ) {
    
}


/** Clone object */
Func_power* Func_power::clone( void ) const {
    
    return new Func_power( *this );
}


RevObject* Func_power::execute() {
    
    RevBayesCore::TypedDagNode<double>* b = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* e = static_cast<const Real &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::PowerFunction* f = new RevBayesCore::PowerFunction( b, e );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    RealPos* value = new RealPos( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Func_power::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "base", true, Real::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "exponent", true, Real::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_power::getClassName(void) { 
    
    static std::string rbClassName = "Func_power";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_power::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_power::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_power::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
