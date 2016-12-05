//
//  GammaFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/21/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "GammaFunction.h"
#include "RbMathFunctions.h"
#include <cmath>

using namespace RevBayesCore;

GammaFunction::GammaFunction(const TypedDagNode<double> *x) : TypedFunction<double>( new double(1.0) ),
a( x )
{
    addParameter( x );
    
}


GammaFunction* GammaFunction::clone( void ) const
{
    
    return new GammaFunction(*this);
    
}


void GammaFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == a)
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void GammaFunction::update( void )
{
    
    *value = RevBayesCore::RbMath::gamma( a->getValue() );
    
}


