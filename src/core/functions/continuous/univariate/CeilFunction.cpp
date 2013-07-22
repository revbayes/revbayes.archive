//
//  Add.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 8/13/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "CeilFunction.h"
#include <cmath>

using namespace RevBayesCore;

CeilFunction::CeilFunction(const TypedDagNode<double> *x) : TypedFunction<int>( new int(0) ), a( x ) {
    addParameter( x );
    
}


CeilFunction* CeilFunction::clone( void ) const {

    return new CeilFunction(*this);

}


void CeilFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {

    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void CeilFunction::update( void ) {
    *value = ceil( a->getValue() );
}


