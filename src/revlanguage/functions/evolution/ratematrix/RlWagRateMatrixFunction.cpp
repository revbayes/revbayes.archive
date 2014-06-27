//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlWagRateMatrixFunction.h"
#include "RateMatrix_Wag.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
WagRateMatrixFunction::WagRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
WagRateMatrixFunction* WagRateMatrixFunction::clone( void ) const {
    
    return new WagRateMatrixFunction( *this );
}


RevObject* WagRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_Wag *rmj = new RevBayesCore::RateMatrix_Wag();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& WagRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& WagRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "Func_wag";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& WagRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& WagRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& WagRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
