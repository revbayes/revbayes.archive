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


/** We catch here the setting of the member variables to store our parameters. */
void InferenceDistributionContinuous::setInternalObservedValue(const RbValue<void *> &v) {
    
    randomVariable.value    = static_cast<double*>( v.value );
    randomVariable.lengths  = v.lengths;
    
}