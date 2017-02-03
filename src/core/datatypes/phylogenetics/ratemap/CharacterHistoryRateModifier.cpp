//
//  CharacterHistoryRateModifier.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/8/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#include "CharacterHistoryRateModifier.h"
#include <iostream>

using namespace RevBayesCore;

CharacterHistoryRateModifier::CharacterHistoryRateModifier(size_t ns, size_t nc) :
    num_states(ns),
    num_characters(nc)
{
    ; // do nothing
}


CharacterHistoryRateModifier::CharacterHistoryRateModifier(const CharacterHistoryRateModifier& g)
{
    
    if (&g != this)
    {
        num_states = g.num_states;
        num_characters = g.num_characters;
    }
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const std::vector<CharacterHistoryRateModifier*>& x)
{
    o << " ";
    
    return o;
}

std::ostream& RevBayesCore::operator<<(std::ostream& o, const CharacterHistoryRateModifier& x)
{
    o << " ";
    
    return o;
}
