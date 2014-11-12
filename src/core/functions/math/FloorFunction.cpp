//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "FloorFunction.h"
#include <cmath>

using namespace RevBayesCore;

FloorFunction::FloorFunction(const TypedDagNode<double> *x) : TypedFunction<int>( new int(0) ), a( x ) {
    addParameter( x );
    
}


FloorFunction* FloorFunction::clone( void ) const {
    
    return new FloorFunction(*this);

}


void FloorFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void FloorFunction::update( void ) 
{
    
    *value = int( floor( a->getValue() ) );

}


