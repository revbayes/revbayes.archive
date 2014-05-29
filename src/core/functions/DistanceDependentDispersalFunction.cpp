//
//  DistanceDependentDispersalFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/17/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "DistanceDependentDispersalFunction.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"
#include <cmath>

using namespace RevBayesCore;

DistanceDependentDispersalFunction::DistanceDependentDispersalFunction(const TypedDagNode<double>* dp, TimeAtlas* ta) : TypedFunction<GeographicDistanceRateModifier>( new GeographicDistanceRateModifier(ta) )
{
    // add the parameters as parents
    distancePower = dp;
    
    addParameter(distancePower);
    
    update();
}


DistanceDependentDispersalFunction::DistanceDependentDispersalFunction(const DistanceDependentDispersalFunction &n) : TypedFunction<GeographicDistanceRateModifier>( n ), distancePower(n.distancePower)
{
    
    // no need to add parameters, happens automatically
    //value = n.value;
    update();
}


DistanceDependentDispersalFunction::~DistanceDependentDispersalFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



DistanceDependentDispersalFunction* DistanceDependentDispersalFunction::clone( void ) const
{
    return new DistanceDependentDispersalFunction( *this );
}


void DistanceDependentDispersalFunction::update( void )
{
    //value->updateGeographicDistancePowers(distancePower->getValue(),true);
    // recompute distances based on distancePower
    
    value->setDistancePower(distancePower->getValue(), true);
}



void DistanceDependentDispersalFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == distancePower)
    {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}
