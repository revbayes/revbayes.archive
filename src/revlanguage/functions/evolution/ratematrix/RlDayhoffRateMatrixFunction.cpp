//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlDayhoffRateMatrixFunction.h"
#include "RateMatrix_Dayhoff.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
DayhoffRateMatrixFunction::DayhoffRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
DayhoffRateMatrixFunction* DayhoffRateMatrixFunction::clone( void ) const {
    
    return new DayhoffRateMatrixFunction( *this );
}


RbLanguageObject* DayhoffRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_Dayhoff *rmj = new RevBayesCore::RateMatrix_Dayhoff();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& DayhoffRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& DayhoffRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "Dayhoff";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& DayhoffRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& DayhoffRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& DayhoffRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
