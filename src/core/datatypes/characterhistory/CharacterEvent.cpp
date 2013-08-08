//
//  CharacterEvent.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//


#include "CharacterEvent.h"
#include <iostream>

using namespace RevBayesCore;


CharacterEvent::CharacterEvent(void)
{
    
}

CharacterEvent::CharacterEvent(DiscreteCharacterState* s, size_t i, double t) : time(t), state(s), index(i)
{
    
}

CharacterEvent::CharacterEvent(const CharacterEvent& c) : time(c.time), state(c.state), index(c.index)
{
    
}

CharacterEvent::~CharacterEvent(void)
{
    delete state;
}

bool CharacterEvent::operator<(const CharacterEvent& rhs) const
{
    return time < rhs.time;
}

double CharacterEvent::getTime(void)
{
    return time;
}

size_t CharacterEvent::getIndex(void)
{
    return index;
}

DiscreteCharacterState* CharacterEvent::getState(void)
{
    return state;
}

void CharacterEvent::setState(DiscreteCharacterState* s)
{
    state = s;
}

void CharacterEvent::print(void)
{
    std::cout << state << " " << index << " " << time << "\n";
}