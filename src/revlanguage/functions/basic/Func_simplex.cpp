//
//  Func_simplex.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/8/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_simplex.h"
#include "ArgumentRule.h"
#include "DeterministicNode.h"
#include "Ellipsis.h"
#include "RbUtil.h"
#include "RealPos.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "Simplex.h"
#include "SimplexFunction.h"


using namespace RevLanguage;

Func_simplex::Func_simplex() : Function() {
    
}

/* Clone object */
Func_simplex* Func_simplex::clone( void ) const {
    
    return new Func_simplex( *this );
}


/** Execute function: We rely on getValue and overloaded push_back to provide functionality */
RbLanguageObject* Func_simplex::execute( void ) {
    
    std::vector<const RevBayesCore::TypedDagNode<double>* > params;
    for ( size_t i = 0; i < args.size(); i++ ) {
        const RealPos &val = static_cast<const RealPos &>( args[i].getVariable()->getValue() );
        params.push_back( val.getValueNode() );
    }
    
    RevBayesCore::SimplexFunction *func = new RevBayesCore::SimplexFunction( params );
    RevBayesCore::TypedDagNode<std::vector<double> > *detNode = new RevBayesCore::DeterministicNode<std::vector<double> >("", func);
    Simplex *theSimplex = new Simplex( detNode );
        
    return theSimplex;
}


/** Get argument rules */
const ArgumentRules& Func_simplex::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new Ellipsis (     RealPos::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get class name of object */
const std::string& Func_simplex::getClassName(void) { 
    
    static std::string rbClassName = "Func_simplex";
    
	return rbClassName; 
}


/** Get class type spec describing type of object */
const RevLanguage::TypeSpec& Func_simplex::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( "Func_simplex", new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/** Get type spec */
const TypeSpec& Func_simplex::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_simplex::getReturnType( void ) const {
    
    return Simplex::getClassTypeSpec();
}
