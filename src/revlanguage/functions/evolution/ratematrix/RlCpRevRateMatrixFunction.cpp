//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlCpRevRateMatrixFunction.h"
#include "RateMatrix_CpRev.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
CpRevRateMatrixFunction::CpRevRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
CpRevRateMatrixFunction* CpRevRateMatrixFunction::clone( void ) const {
    
    return new CpRevRateMatrixFunction( *this );
}


RbLanguageObject* CpRevRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_CpRev *rmj = new RevBayesCore::RateMatrix_CpRev();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& CpRevRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& CpRevRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "CpRev";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& CpRevRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& CpRevRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& CpRevRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
