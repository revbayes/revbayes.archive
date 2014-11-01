//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "RoundFunction.h"
#include <cmath>

using namespace RevBayesCore;

RoundFunction::RoundFunction(const TypedDagNode<double> *x) : TypedFunction<int>( new int(0) ), a( x ) 
{
    
    addParameter( x );
    
}


RoundFunction* RoundFunction::clone( void ) const 
{
    
    return new RoundFunction(*this);
}


void RoundFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void RoundFunction::update( void ) 
{
    
    *value = int( round( a->getValue() ) );
    
}


