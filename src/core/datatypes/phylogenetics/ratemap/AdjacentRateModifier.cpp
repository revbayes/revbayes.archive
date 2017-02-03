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

AdjacentRateModifier::AdjacentRateModifier(size_t ns, size_t nc) : CharacterHistoryRateModifier(ns, nc)
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
    size_t num_match = 0;
    
    size_t to_index = newState->getSiteIndex();
    size_t to_state = newState->getState();
    
    int lower_bound = ( (int)(to_index - width) < 0                     ? 0                               : (int)(to_index - width) );
    int upper_bound = ( (int)(to_index + width) >= this->num_characters ? (int)(this->num_characters - 1) : (int)(to_index + width) );
    
//    std::cout << to_index << ":\n";
    for (int i = lower_bound; i <= upper_bound; i++)
    {
        if (i == to_index)
            continue;
        else if (currState[i]->getState() == to_state)
        {
            num_match++;
        }
//        size_t from_state = currState[i]->getState();
//        std::cout << i << " " << from_state << " " << to_state << "\n";
    }
    
    double r = exp(num_match * factor);
//    std::cout << r << "\n\n";
    return 1.0;
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

void AdjacentRateModifier::setFactor(double f)
{
    factor = f;
}

void AdjacentRateModifier::setWidth(size_t w)
{
    width = w;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const AdjacentRateModifier& x)
{
    o << "AdjacentRateModifier";
    return o;
}
