//
//  ChooseFunction.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/21/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "ChooseFunction.h"
#include "RbMathFunctions.h"
#include "RbMathCombinatorialFunctions.h"
#include <cmath>

using namespace RevBayesCore;

ChooseFunction::ChooseFunction(const TypedDagNode<int> *a, const TypedDagNode<int> *b) : TypedFunction<int>( new int(0) ),
n( a ),
k( b )
{
    addParameter( n );
    addParameter( k );
    
}


ChooseFunction* ChooseFunction::clone( void ) const
{
    
    return new ChooseFunction(*this);
    
}


void ChooseFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == n)
    {
        n = static_cast<const TypedDagNode<int>* >( newP );
    }
    else if (oldP == k)
    {
        k = static_cast<const TypedDagNode<int>* >( newP );
    }
    
}

void ChooseFunction::update( void )
{
    
    *value = RevBayesCore::RbMath::choose( n->getValue(), k->getValue() );
    
}
