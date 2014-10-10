//
//  CharacterEvent.cpp
//  rb_mlandis
//
//  Created by Michael Landis on 8/6/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//


#include "CharacterEvent.h"
#include <iostream>
#include <sstream>

using namespace RevBayesCore;


CharacterEvent::CharacterEvent(void)
{
    
}

CharacterEvent::CharacterEvent(size_t i, unsigned int s, double t) :  index(i), state(s), time(t)
{
    
}

CharacterEvent::CharacterEvent(const CharacterEvent& c) : index(c.index), state(c.state), time(c.time)
{
    
}

CharacterEvent::~CharacterEvent(void)
{

}

CharacterEvent* CharacterEvent::clone( void ) const
{
    return new CharacterEvent( *this );
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

unsigned int CharacterEvent::getState(void)
{
    return state;
}

std::string CharacterEvent::getStateStr(void)
{
    std::stringstream ss;
    // want to do this based on DiscreteDataType...
    ss << (char)(70+state);
    return ss.str();
}

void CharacterEvent::setIndex(size_t i)
{
    index = i;
}

void CharacterEvent::setState(unsigned int s)
{
    state = s;
}

void CharacterEvent::setTime(double t)
{
    time = t;
}

void CharacterEvent::print(void)
{
    std::cout << index << " " << state << " " << time << "\n";
}
