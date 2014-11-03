//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AbsoluteValueFunction.h"
#include <cmath>

using namespace RevBayesCore;

AbsoluteValueFunction::AbsoluteValueFunction(const TypedDagNode<double> *x) : ContinuousFunction( new double(0.0) ),
    a( x )
{
    
    addParameter( x );
    
}


AbsoluteValueFunction* AbsoluteValueFunction::clone( void ) const
{

    return new AbsoluteValueFunction(*this);
}


void AbsoluteValueFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == a)
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void AbsoluteValueFunction::update( void )
{
    *value = fabs( a->getValue() );
}


