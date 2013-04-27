//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlBlosum62RateMatrixFunction.h"
#include "RateMatrix_Blosum62.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "Simplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Blosum62RateMatrixFunction::Blosum62RateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
Blosum62RateMatrixFunction* Blosum62RateMatrixFunction::clone( void ) const {
    
    return new Blosum62RateMatrixFunction( *this );
}


RbLanguageObject* Blosum62RateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_Blosum62 *rmj = new RevBayesCore::RateMatrix_Blosum62();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& Blosum62RateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& Blosum62RateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "Blosum62";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& Blosum62RateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& Blosum62RateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& Blosum62RateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
