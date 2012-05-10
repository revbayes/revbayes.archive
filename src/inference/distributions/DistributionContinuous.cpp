//
//  DistributionContinuous.cpp
//  RevBayes
//
//  Created by Sebastian Hoehna on 4/17/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "DistributionContinuous.h"
#include "RbConstants.h"

double DistributionContinuous::getMax(void) const {
    return RbConstants::Double::inf;
}


double DistributionContinuous::getMin(void) const {
    return -RbConstants::Double::inf;
}


/** We catch here the setting of the member variables to store our parameters. */
void DistributionContinuous::setInternalObservedValue(const RbValue<void *> &v) {
    
    randomVariable.value    = static_cast<double*>( v.value );
    randomVariable.lengths  = v.lengths;
    
}