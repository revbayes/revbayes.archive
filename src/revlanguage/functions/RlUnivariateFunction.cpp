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



UnivariateFunction::~UnivariateFunction() {
    
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

