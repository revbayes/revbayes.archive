//
//  RateMap_Biogeography.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 4/2/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#include "RateMap_Biogeography.h"
#include <cmath>

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

double RateMap_Biogeography::getRate(std::vector<CharacterEvent*> from, CharacterEvent* to) const
{
    double rate = 0.0;
    int s = to->getState();
    int n1 = numOn(from);
    
    // rate to extinction cfg is 0
    if (n1 == 1 && s == 0)
        return 0.0;
    
    // rate according to binary rate matrix Q
    rate = gainLossRates[s];
    
    return rate;

}

double RateMap_Biogeography::getSumOfRates(std::vector<CharacterEvent*> from) const
{
    // get rate away away from currState
    size_t n1 = numOn(from);
    size_t n0 = numCharacters - n1;
    
    // forbid extinction events
    if (n1 == 1)
        n1 = 0;
    
    double r0 = n1 * gainLossRates[0];
    double r1 = n0 * gainLossRates[1];
    double sum = r0 + r1;
    
    return sum;
}


double RateMap_Biogeography::getTransitionProbability(CharacterState* from, CharacterState* to, double t) const
{
    return 1.0;
}

double RateMap_Biogeography::getLnTransitionProbability(CharacterState* from, CharacterState* to, double t) const
{
    return 0.0;
}

double RateMap_Biogeography::getLnTransitionProbability(std::vector<CharacterEvent*> from, CharacterEvent* to, double t) const
{
    double lnP = log(getRate(from, to)) - getSumOfRates(from) * t;
    return lnP;
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

size_t RateMap_Biogeography::numOn(const std::vector<CharacterEvent*>& s) const
{
    size_t n = 0;
    for (size_t i = 0; i < s.size(); i++)
        if (s[i]->getState() == 1)
            n++;
    return n;
}

