//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MeanFunction.h"
#include "Func_mean.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "Vector.h"

using namespace RevLanguage;

/** default constructor */
Func_mean::Func_mean( void ) : Function( ) {
    
}


/** Clone object */
Func_mean* Func_mean::clone( void ) const {
    
    return new Func_mean( *this );
}


RevPtr<Variable> Func_mean::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* arg = static_cast<const Vector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MeanFunction* f = new RevBayesCore::MeanFunction( arg );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    Real* value = new Real( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x", true, Vector<Real>::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mean::getClassName(void) { 
    
    static std::string rbClassName = "Func_mean";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mean::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_mean::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_mean::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
