//
//  ContinuousDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RlContinuousDistribution.h"

#include "ContinuousStochasticNode.h"
#include "TypedDistribution.h"

using namespace RevLanguage;

ContinuousDistribution::ContinuousDistribution() : TypedDistribution<Real>() {
    
}



ContinuousDistribution::ContinuousDistribution( const ContinuousDistribution &d ) : TypedDistribution<Real>(d) {
    
}



ContinuousDistribution::~ContinuousDistribution() {
    
}



Real* ContinuousDistribution::createRandomVariable(void) const { 
    
    RevBayesCore::ContinuousDistribution* d = createDistribution();
    RevBayesCore::TypedDagNode<double>* rv  = new RevBayesCore::ContinuousStochasticNode("", d);
    
    return new Real(rv);
}



/* Get class name of object */
const std::string& ContinuousDistribution::getClassType(void) { 
    
    static std::string rbClassName = "ContinuousDistribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& ContinuousDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<Real>::getClassTypeSpec() ) );
    
	return rbClass; 
}

