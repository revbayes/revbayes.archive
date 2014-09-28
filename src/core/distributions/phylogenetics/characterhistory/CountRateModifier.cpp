//
//  CountRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "CountRateModifier.h"
#include "RbConstants.h"

using namespace RevBayesCore;

CountRateModifier::CountRateModifier(unsigned nc)

{
    // initialize
    numCharacters = nc;
    setProportions(std::vector<double>(nc, 1.0/nc));
//    update();
}

CountRateModifier::CountRateModifier(const CountRateModifier& g)
{
    
    if (&g != this)
    {
        numCharacters = g.numCharacters;
        proportions = g.proportions;
        setProportions(proportions);
    }
}

double CountRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    double r = 1.0;
    
    std::map<unsigned, unsigned> m;
    for (size_t i = 0; i < currState.size(); i++)
    {
        
    }
    
    return r;
}

double CountRateModifier::computeRateModifier(const TopologyNode& node, std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    return computeRateModifier(currState, newState, age);
}

double CountRateModifier::computeRateModifier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    return computeRateModifier(currState, newState, 0.0);
}

double CountRateModifier::computeSiteRateModifier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    return 1.0;
}

double CountRateModifier::computeSiteRateModifier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    return 1.0;
}


CountRateModifier* CountRateModifier::clone(void) const
{
    return new CountRateModifier(*this);
}



void CountRateModifier::update(void)
{
    ; // do nothing
}

void CountRateModifier::setProportions(const std::vector<double>& p)
{
    proportions = p;
}

const std::vector<double>& CountRateModifier::getProportions(void) const
{
    return proportions;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const CountRateModifier& x)
{
    o << "CountRateModifier";
    return o;
}
