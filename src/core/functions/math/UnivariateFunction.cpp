//
//  UnivariateFunction.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 9/5/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "UnivariateFunction.h"


using namespace RevBayesCore;


UnivariateFunction::UnivariateFunction(double *v, const TypedDagNode<double> *z ) : ContinuousFunction( v ), x( z ) {
    addParameter( x );
}


const TypedDagNode<double>* UnivariateFunction::getParameter( void ) const {
    return x;
}


void UnivariateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == x) {
        x = static_cast<const TypedDagNode<double>* >( newP );
    }
}
