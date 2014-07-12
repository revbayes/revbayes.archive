//
//  ContinuousDistribution.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 11/16/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RlPositiveContinuousDistribution.h"

#include "ContinuousStochasticNode.h"
#include "TypedDistribution.h"

using namespace RevLanguage;

PositiveContinuousDistribution::PositiveContinuousDistribution() : TypedDistribution<RealPos>() {
    
}



PositiveContinuousDistribution::PositiveContinuousDistribution( const PositiveContinuousDistribution &d ) : TypedDistribution<RealPos>(d) {
    
}



PositiveContinuousDistribution::~PositiveContinuousDistribution() {
    
}



RealPos* PositiveContinuousDistribution::createRandomVariable(void) const { 
    
    RevBayesCore::ContinuousDistribution* d = createDistribution();
    RevBayesCore::TypedDagNode<double>* rv  = new RevBayesCore::ContinuousStochasticNode("", d);
    
    return new RealPos(rv);
}



/* Get class name of object */
const std::string& PositiveContinuousDistribution::getClassType(void) { 
    
    static std::string rbClassName = "PositiveContinuousDistribution";
    
	return rbClassName; 
}

/* Get class type spec describing type of object */
const TypeSpec& PositiveContinuousDistribution::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<RealPos>::getClassTypeSpec() ) );
    
	return rbClass; 
}

