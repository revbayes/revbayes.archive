//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MeanFunction.h"
#include "Func_mean.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_mean::Func_mean( void ) : Function( ) {
    
}


/** Clone object */
Func_mean* Func_mean::clone( void ) const {
    
    return new Func_mean( *this );
}


RevPtr<Variable> Func_mean::execute() {
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MeanFunction* f = new RevBayesCore::MeanFunction( arg );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    Real* value = new Real( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_mean::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_mean::getClassType(void) { 
    
    static std::string revType = "Func_mean";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_mean::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
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
