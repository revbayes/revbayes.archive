//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "ExponentialFunction.h"
#include <cmath>

using namespace RevBayesCore;

ExponentialFunction::ExponentialFunction(const TypedDagNode<double> *x, std::string revDecl) : ContinuousFunction( new double(0.0), revDecl ), a( x ) {
    addParameter( x );

}


ExponentialFunction* ExponentialFunction::clone( void ) const {
    
    return new ExponentialFunction(*this);
}


void ExponentialFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void ExponentialFunction::update( void ) {
    
    *value = exp( a->getValue() );
}


