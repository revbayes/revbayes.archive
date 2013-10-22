//
//  RangeSizeRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 10/18/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "RangeSizeRateModifier.h"

#include <cmath>
#include "CharacterEvent.h"
#include "RbConstants.h"

using namespace RevBayesCore;

RangeSizeRateModifier::RangeSizeRateModifier(unsigned int na, unsigned int sm) : numAreas(na), stateModified(sm)
{
    
    update();
    
}

double RangeSizeRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    int n = 0;
    for (size_t i = 0; i < numAreas; i++)
        if (currState[i]->getState() == stateModified)
            n++;
    
    double ret = 1.0 / pow(1.0 + fabs(n - frequency * numAreas), power);
    return ret;
}

RangeSizeRateModifier* RangeSizeRateModifier::clone(void) const
{
    return new RangeSizeRateModifier(*this);
}



void RangeSizeRateModifier::update(void)
{
    ;
}


void RangeSizeRateModifier::updateFrequency(double f)
{
    frequency = f;
}

void RangeSizeRateModifier::updatePower(double p)
{
    power = p;
}