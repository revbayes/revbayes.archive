//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "HkyRateMatrixFunction.h"
#include "RlHkyRateMatrixFunction.h"
#include "RateMatrix_HKY.h"
#include "RateValueMatrix.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "Simplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
HkyRateMatrixFunction::HkyRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
HkyRateMatrixFunction* HkyRateMatrixFunction::clone( void ) const {
    
    return new HkyRateMatrixFunction( *this );
}


RbLanguageObject* HkyRateMatrixFunction::execute() {
    
    RevBayesCore::TypedDagNode< double >* ka = static_cast<const RealPos &>( this->args[0].getVariable()->getValue() ).getValueNode();
    RevBayesCore::TypedDagNode<std::vector<double> >* bf = static_cast<const Simplex &>( this->args[1].getVariable()->getValue() ).getValueNode();
    RevBayesCore::HkyRateMatrixFunction* f = new RevBayesCore::HkyRateMatrixFunction( ka, bf );
    RevBayesCore::DeterministicNode<RevBayesCore::RateValueMatrix> *detNode = new RevBayesCore::DeterministicNode<RevBayesCore::RateValueMatrix>("", f);
    
    RateMatrix* value = new RateMatrix( detNode );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& HkyRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "kappa", true, RealPos::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "baseFrequencies", true, Simplex::getClassTypeSpec() ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& HkyRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "HKY";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& HkyRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& HkyRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& HkyRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
