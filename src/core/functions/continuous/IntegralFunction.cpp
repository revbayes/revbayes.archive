//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "IntegralFunction.h"
#include <cmath>

using namespace RevBayesCore;

IntegralFunction::IntegralFunction(const TypedDagNode<IntegrableFunction> *f, const TypedDagNode<double> *l, const TypedDagNode<double> *u) : ContinuousFunction( new double(0.0) ), function( f ), lower( l ), upper( u ) {
    this->addParameter( f );
    this->addParameter( l );
    this->addParameter( u );

}


IntegralFunction* IntegralFunction::clone( void ) const {
    return new IntegralFunction(*this);
}

void IntegralFunction::update( void ) {
    *value = function->getValue().integrate(lower->getValue(), upper->getValue());
}


void IntegralFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if ( oldP == function ) {
        function = static_cast<const TypedDagNode<IntegrableFunction>*>( newP );
    }
    if ( oldP == lower ) {
        lower = static_cast<const TypedDagNode<double>*>( newP );
    }
    if ( oldP == upper ) {
        upper = static_cast<const TypedDagNode<double>*>( newP );
    }
}


