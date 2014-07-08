//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_sqrt.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "SqrtFunction.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sqrt::Func_sqrt( void ) : Function( ) {
    
}


/** Clone object */
Func_sqrt* Func_sqrt::clone( void ) const {
    
    return new Func_sqrt( *this );
}


RevPtr<Variable> Func_sqrt::execute() {
    
    RevBayesCore::TypedDagNode<double>* arg = static_cast<const Real &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SqrtFunction* f = new RevBayesCore::SqrtFunction( arg );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    RealPos* value = new RealPos( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_sqrt::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, RealPos::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_sqrt::getClassName(void) { 
    
    static std::string rbClassName = "Func_sqrt";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_sqrt::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_sqrt::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_sqrt::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
