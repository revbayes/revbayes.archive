//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "StepFunction.h"
#include "RbException.h"
#include "RbUtil.h"
#include <stdio.h>

using namespace RevBayesCore;

StepFunction::StepFunction(double *val, const TypedDagNode<double> *x, const std::vector<double> &vals) : UnivariateFunction( val, x ), values( vals ) {
    
}


//const std::vector<double>& StepFunction::getIntervals( void ) const {
//    return intervals;
//}


std::vector<double>& StepFunction::getPathValues( void ) {
    return values;
}


const std::vector<double>& StepFunction::getPathValues( void ) const {
    return values;
}


///** Stepfunctions are always discrete value function.
// */
//bool StepFunction::isDiscrete( void ) const {
//    return true;
//}
//
//
//void StepFunction::setPathValues( const std::vector<double>& x ) {
//    values->getValue() = x;
//}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const StepFunction& f) {
    
    o << "Step Function";
    
    return o;
}

