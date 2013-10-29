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

RangeSizeRateModifier::RangeSizeRateModifier(unsigned int na, unsigned int sm, int sgn) : numAreas(na), stateModified(sm), sign(sgn)
{
    
    update();
}

RangeSizeRateModifier::RangeSizeRateModifier(const RangeSizeRateModifier& m) : numAreas(m.numAreas), stateModified(m.stateModified), sign(m.sign)
{
    ;
}

double RangeSizeRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    return computeRateModifier(currState, newState->getState());
}

double RangeSizeRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, int newState)
{
    int n = 0;
    for (size_t i = 0; i < numAreas; i++)
        if (newState == stateModified)
            n++;
    
 //   double ret = 1.0 / pow(1.0 + fabs(n - frequency * numAreas), power);
 //   std::cout << newState << " " << n << " " << frequency << " " << power << " " << ret << "\n";
    
    double ret = intercept + sign * n * slope;
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
//    frequency = f;
    if (stateModified == 0)
        frequency = 1.0 - f;
    else
        frequency = f;
}

void RangeSizeRateModifier::updatePower(double p)
{
    power = p;
}
