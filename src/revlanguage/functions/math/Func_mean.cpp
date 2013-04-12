//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "MeanFunction.h"
#include "Func_mean.h"
#include "Real.h"
#include "RealPos.h"
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


RbLanguageObject* Func_mean::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* arg = static_cast<const Vector<Real> &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::MeanFunction* f = new RevBayesCore::MeanFunction( arg );
    RevBayesCore::DeterministicNode<double> *detNode = new RevBayesCore::DeterministicNode<double>("", f);
    
    Real* value = new Real( detNode );
    
    return value;
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
    
    static std::string rbClassName = "mean";
    
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
