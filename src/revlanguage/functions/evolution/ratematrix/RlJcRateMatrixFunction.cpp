//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "JcRateMatrixFunction.h"
#include "Natural.h"
#include "RlJcRateMatrixFunction.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
JcRateMatrixFunction::JcRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
JcRateMatrixFunction* JcRateMatrixFunction::clone( void ) const {
    
    return new JcRateMatrixFunction( *this );
}


RevObject* JcRateMatrixFunction::execute() {
    
    int ns = static_cast<const Natural &>( this->args[0].getVariable()->getValue() ).getValue();
    RevBayesCore::JcRateMatrixFunction* f = new RevBayesCore::JcRateMatrixFunction( size_t(ns) );
    RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateMatrix>("", f);
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& JcRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "numStates", true, Natural::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& JcRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "JC";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& JcRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& JcRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& JcRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
