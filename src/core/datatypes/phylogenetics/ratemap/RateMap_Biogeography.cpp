//
//  RateMap_Biogeography.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMap_Biogeography.h"

using namespace RevBayesCore;

RateMap_Biogeography::RateMap_Biogeography(size_t nc) : RateMap(2, nc)
{
    
}

RateMap_Biogeography::RateMap_Biogeography(const RateMap_Biogeography& m) : RateMap( m )
{
    
}

RateMap_Biogeography::~RateMap_Biogeography(void)
{
    
}

RateMap_Biogeography& RateMap_Biogeography::operator=(const RateMap_Biogeography& r)
{
    if (this != &r)
    {
        RateMap::operator=( r );
    }
    return *this;
}

RateMap_Biogeography* RateMap_Biogeography::clone(void) const
{
    return new RateMap_Biogeography( *this );
}

double RateMap_Biogeography::getRate(CharacterState* from, CharacterState* to) const
{
    return 1.0;
}

double RateMap_Biogeography::getSumOfRates(CharacterState* from) const
{
    return 1.0;
}

double RateMap_Biogeography::getTransitionProbability(CharacterState* from, CharacterState* to, double t) const
{
    return 1.0;
}

double RateMap_Biogeography::getLnTransitionProbability(CharacterState* from, CharacterState* to, double t) const
{
    return 0.0;
}

void RateMap_Biogeography::updateMap(void)
{
    ; // do nothing
}


const std::vector<double>& RateMap_Biogeography::getGainLossRates(void) const
{
    return gainLossRates;
}

void RateMap_Biogeography::setGainLossRates(const std::vector<double> &r)
{
    gainLossRates = r;
}

double RateMap_Biogeography::getDistancePower(void) const
{
    return distancePower;
}

void RateMap_Biogeography::setDistancePower(double d)
{
    distancePower = d;
}
