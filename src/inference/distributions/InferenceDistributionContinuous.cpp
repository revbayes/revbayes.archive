//
//  InferenceDistributionContinuous.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "InferenceDistributionContinuous.h"
#include "RbConstants.h"

double InferenceDistributionContinuous::getMax(void) const {
    return RbConstants::Double::inf;
}


double InferenceDistributionContinuous::getMin(void) const {
    return -RbConstants::Double::inf;
}