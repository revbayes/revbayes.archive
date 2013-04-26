//
//  ContinuousDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#include "RlUnivariateFunction.h"
#include "TypedFunction.h"

using namespace RevLanguage;

UnivariateFunction::UnivariateFunction() : TypedFunction<Real>() {
    
}



UnivariateFunction::UnivariateFunction( const UnivariateFunction &d ) : TypedFunction<Real>(d) {
    
}


UnivariateFunction::UnivariateFunction(RevBayesCore::TypedDagNode<RevBayesCore::UnivariateFunction> *n) {
    throw RbException("Missing implementation in constructor of UnivariateFunction");
}



UnivariateFunction::~UnivariateFunction() {
    
}



UnivariateFunction* UnivariateFunction::clone( void ) const {
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in UnivariateFunction::clone");
    
    return NULL;
}


RevBayesCore::UnivariateFunction* UnivariateFunction::createFunction(void) const {
    
    // todo: implement this or find better solution (Sebastian)
    throw RbException("Missing implemention in UnivariateFunction::createFunction");
    
    return NULL;
}


/* Get argument rules */
const ArgumentRules& UnivariateFunction::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    
    return argumentRules;
}



/* Get class name of object */
const std::string& UnivariateFunction::getClassName(void) { 
    
    static std::string rbClassName = "UnivariateFunction";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& UnivariateFunction::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( TypedFunction<Real>::getClassTypeSpec() ) );
    
	return rbClass; 
}




/* Get return type */
const TypeSpec& UnivariateFunction::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Real::getClassTypeSpec();
    
    return returnTypeSpec;
}



const TypeSpec& UnivariateFunction::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
