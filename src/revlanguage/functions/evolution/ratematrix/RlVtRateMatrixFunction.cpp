//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlVtRateMatrixFunction.h"
#include "RateMatrix_Vt.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
VtRateMatrixFunction::VtRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
VtRateMatrixFunction* VtRateMatrixFunction::clone( void ) const {
    
    return new VtRateMatrixFunction( *this );
}


RbLanguageObject* VtRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_Vt *rmj = new RevBayesCore::RateMatrix_Vt();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& VtRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& VtRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "Vt";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& VtRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& VtRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& VtRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
