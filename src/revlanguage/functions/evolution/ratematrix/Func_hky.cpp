//
//  Func_hky.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Func_hky.h"
#include "HkyRateMatrixFunction.h"
#include "RateMatrix_HKY.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_hky::Func_hky( void ) : Function( ) {
    
}


/** Clone object */
Func_hky* Func_hky::clone( void ) const {
    
    return new Func_hky( *this );
}


RevPtr<Variable> Func_hky::execute() {
    
    RevBayesCore::TypedDagNode< double >* ka = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::HkyRateMatrixFunction* f = new RevBayesCore::HkyRateMatrixFunction( ka, bf );

    DeterministicNode<RevBayesCore::RateMatrix> *detNode = new DeterministicNode<RevBayesCore::RateMatrix>("", f, this->clone());
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_hky::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "kappa", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_hky::getClassName(void) { 
    
    static std::string rbClassName = "Func_hky";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_hky::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_hky::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_hky::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
