//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "MinFunction.h"
#include "Func_min.h"
#include "ModelVector.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_min::Func_min( void ) : Function( ) {
    
}


/** Clone object */
Func_min* Func_min::clone( void ) const {
    
    return new Func_min( *this );
}


RevPtr<Variable> Func_min::execute() {
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::MinFunction* f = new RevBayesCore::MinFunction( arg );
    
    DeterministicNode<double> *detNode = new DeterministicNode<double>("", f, this->clone());
    
    Real* value = new Real( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_min::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_min::getClassType(void) {
    
    static std::string revType = "Func_min";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_min::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/* Get return type */
const TypeSpec& Func_min::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_min::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
