//
//  QuantileFunction.cpp
//  RevBayesCore
//
//  Created by Sebastian Hoehna on 12/1/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "QuantileFunction.h"


RevBayesCore::QuantileFunction::QuantileFunction(const TypedDagNode<double> *q, ContinuousDistribution* d) : ContinuousFunction( new double(0.0) ),
    p( q ),
    dist( d )
{
    
    addParameter( p );
    
    const std::set<const DagNode*>& params = dist->getParameters();
    for (std::set<const DagNode* >::const_iterator it = params.begin(); it != params.end(); ++it) 
    {
        addParameter( *it );
    }
    
}


RevBayesCore::QuantileFunction::QuantileFunction(const QuantileFunction &qf) : ContinuousFunction( qf ),
    p( qf.p ),
    dist( qf.dist->clone() )
{
    
}

RevBayesCore::QuantileFunction::~QuantileFunction(void) {
    
    delete dist;

}


RevBayesCore::QuantileFunction* RevBayesCore::QuantileFunction::clone( void ) const {
    
    return new QuantileFunction(*this);
}


void RevBayesCore::QuantileFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == p) 
    {
        p = static_cast<const TypedDagNode<double>* >( newP );
    }
    else 
    {
        dist->swapParameter(oldP, newP);
    }
    
}

void RevBayesCore::QuantileFunction::update( void ) {
    
    *value = dist->quantile( p->getValue() );
}
