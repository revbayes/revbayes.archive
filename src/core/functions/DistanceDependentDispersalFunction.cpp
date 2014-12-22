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

DistanceDependentDispersalFunction::DistanceDependentDispersalFunction(const TypedDagNode<double>* dp, const TimeAtlas* ta, bool uadj, bool uav, bool udd) : TypedFunction<GeographyRateModifier>( new GeographyRateModifier(ta, uadj, uav, udd) ), distancePower(dp)
{
    // add the parameters as parents
    addParameter(distancePower);
    
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
    // recompute distances based on distancePower
    double dp = distancePower->getValue();
    
    value->setDistancePower(dp, true);
}



void DistanceDependentDispersalFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == distancePower)
    {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}
