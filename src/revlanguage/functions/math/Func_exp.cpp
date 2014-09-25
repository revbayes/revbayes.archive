//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ExponentialFunction.h"
#include "Func_exp.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_exp::Func_exp( void ) : Function( ) {
    
}


/** Clone object */
Func_exp* Func_exp::clone( void ) const {
    
    return new Func_exp( *this );
}


RevPtr<Variable> Func_exp::execute() {
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::ExponentialFunction* f = new RevBayesCore::ExponentialFunction( arg );
    
    DeterministicNode<double>* detNode = new DeterministicNode<double>("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_exp::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", Real::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );

        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_exp::getClassType(void) { 
    
    static std::string revType = "Func_exp";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_exp::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/* Get return type */
const TypeSpec& Func_exp::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_exp::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
