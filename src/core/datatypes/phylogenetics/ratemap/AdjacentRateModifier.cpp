//
//  AdjacentRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include <iomanip>
#include <cmath>
#include "CharacterEvent.h"
#include "AdjacentRateModifier.h"
#include "RbConstants.h"

using namespace RevBayesCore;

AdjacentRateModifier::AdjacentRateModifier(size_t ns, size_t nc, size_t w, double f) : CharacterHistoryRateModifier(ns, nc),
    width(w),
    factor(f)

{

}

AdjacentRateModifier::AdjacentRateModifier(const AdjacentRateModifier& g) : CharacterHistoryRateModifier(g)
{
    
    if (&g != this)
    {
        width = g.width;
        factor = g.factor;
    }
}

double AdjacentRateModifier::computeRateMultiplier(std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    double r = 1.0;
    
    size_t num_match = 0;
    
    size_t to_index = newState->getSiteIndex();
    size_t to_state = newState->getState();
    
    int lower_bound = 0;
    int upper_bound = (int)(this->num_characters - 1);
    
    for (int i = (int)(to_index - width); i < i + width; i++)
    {
        if ( i < lower_bound || i > upper_bound )
        {
            continue;
        }
        else if (currState[to_index]->getState() == to_state)
        {
            num_match++;
        }
    }
    
    r *= num_match * factor;
    
    return r;
}

double AdjacentRateModifier::computeRateMultiplier(const TopologyNode& node, std::vector<CharacterEvent *> currState, CharacterEvent* newState, double age)
{
    return computeRateMultiplier(currState, newState, age);
}

double AdjacentRateModifier::computeRateMultiplier(std::vector<CharacterEvent *> currState, CharacterEvent* newState)
{
    return computeRateMultiplier(currState, newState, 0.0);
}

double AdjacentRateModifier::computeSiteRateMultiplier(const TopologyNode& node, CharacterEvent* currState, CharacterEvent* newState, double age)
{
    return 1.0;
}

double AdjacentRateModifier::computeSiteRateMultiplier(const TopologyNode& node, unsigned from, unsigned to, unsigned charIdx, double age)
{
    return 1.0;
}


AdjacentRateModifier* AdjacentRateModifier::clone(void) const
{
    return new AdjacentRateModifier(*this);
}

void AdjacentRateModifier::update(void)
{
    ; // do nothing
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AdjacentRateModifier& x)
{
    o << "AdjacentRateModifier";
    return o;
}
