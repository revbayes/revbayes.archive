//
//  GeographyRateModifierFunction.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 5/17/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "GeographyRateModifierFunction.h"
#include "GeographyRateModifier.h"
#include "TimeAtlas.h"
#include "TypedDagNode.h"
#include <cmath>

using namespace RevBayesCore;

GeographyRateModifierFunction::GeographyRateModifierFunction(const TypedDagNode<double>* dp, const TimeAtlas* ta, bool uadj, bool uav, bool udd) : TypedFunction<CharacterHistoryRateModifier>( new GeographyRateModifier(ta, uadj, uav, udd) ), distancePower(dp)
{
    // add the parameters as parents
    addParameter(distancePower);
    
    update();
}


GeographyRateModifierFunction::~GeographyRateModifierFunction( void )
{
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



GeographyRateModifierFunction* GeographyRateModifierFunction::clone( void ) const
{
    return new GeographyRateModifierFunction( *this );
}


void GeographyRateModifierFunction::update( void )
{
    // recompute distances based on distancePower
    double dp = distancePower->getValue();
    
    static_cast<GeographyRateModifier*>(value)->setDistancePower(dp, true);
}



void GeographyRateModifierFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == distancePower)
    {
        distancePower = static_cast<const TypedDagNode<double>* >( newP );
    }
}
