//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlJonesRateMatrixFunction.h"
#include "RateMatrix_Jones.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
JonesRateMatrixFunction::JonesRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
JonesRateMatrixFunction* JonesRateMatrixFunction::clone( void ) const {
    
    return new JonesRateMatrixFunction( *this );
}


RevObject* JonesRateMatrixFunction::execute() {
    

    RevBayesCore::RateMatrix_Jones *rmj = new RevBayesCore::RateMatrix_Jones();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& JonesRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& JonesRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "Func_jones";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& JonesRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& JonesRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& JonesRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
