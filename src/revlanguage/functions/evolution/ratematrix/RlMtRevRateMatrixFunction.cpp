//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlMtRevRateMatrixFunction.h"
#include "RateMatrix_MtRev24.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
MtRevRateMatrixFunction::MtRevRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
MtRevRateMatrixFunction* MtRevRateMatrixFunction::clone( void ) const {
    
    return new MtRevRateMatrixFunction( *this );
}


RbLanguageObject* MtRevRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_MtRev24 *rmj = new RevBayesCore::RateMatrix_MtRev24();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& MtRevRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& MtRevRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "MtRev";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& MtRevRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& MtRevRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& MtRevRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
