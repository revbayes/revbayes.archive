//
//  Func_exp.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/7/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DeterministicNode.h"
#include "Natural.h"
#include "RlRtRevRateMatrixFunction.h"
#include "RateMatrix_RtRev.h"
#include "Real.h"
#include "RealPos.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
RtRevRateMatrixFunction::RtRevRateMatrixFunction( void ) : Function( ) {
    
}


/** Clone object */
RtRevRateMatrixFunction* RtRevRateMatrixFunction::clone( void ) const {
    
    return new RtRevRateMatrixFunction( *this );
}


RbLanguageObject* RtRevRateMatrixFunction::execute() {
    
    
    RevBayesCore::RateMatrix_RtRev *rmj = new RevBayesCore::RateMatrix_RtRev();
    RateMatrix* value = new RateMatrix( rmj );
    
    return value;
}


/* Get argument rules */
const ArgumentRules& RtRevRateMatrixFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}


const std::string& RtRevRateMatrixFunction::getClassName(void) { 
    
    static std::string rbClassName = "RtRev";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& RtRevRateMatrixFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return rbClass; 
}


/* Get return type */
const TypeSpec& RtRevRateMatrixFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RateMatrix::getClassTypeSpec();
    
    return returnTypeSpec;
}


const TypeSpec& RtRevRateMatrixFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
