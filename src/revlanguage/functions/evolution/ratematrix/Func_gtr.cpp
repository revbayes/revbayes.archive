//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GtrRateMatrixFunction.h"
#include "Func_gtr.h"
#include "RateMatrix_GTR.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_gtr::Func_gtr( void ) : Function( ) {
    
}


/** Clone object */
Func_gtr* Func_gtr::clone( void ) const {
    
    return new Func_gtr( *this );
}


RevPtr<Variable> Func_gtr::execute() {
    
    RevBayesCore::TypedDagNode<std::vector<double> >* er = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::GtrRateMatrixFunction* f = new RevBayesCore::GtrRateMatrixFunction( er, bf );

    DeterministicNode<RevBayesCore::RateMatrix> *detNode = new DeterministicNode<RevBayesCore::RateMatrix>("", f, this->clone());
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return new Variable( value );
}


/* Get argument rules */
const ArgumentRules& Func_gtr::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "exchangeRates", true, Simplex::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_gtr::getClassName(void) { 
    
    static std::string rbClassName = "Func_gtr";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_gtr::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Func_gtr::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Func_gtr::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
