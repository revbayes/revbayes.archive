#include "CharacterEvent.h"
#include <iostream>
#include <sstream>

using namespace RevBayesCore;


CharacterEvent::CharacterEvent(void)
{
    
}

CharacterEvent::CharacterEvent(size_t i, size_t s, double t) :
    character_index(i),
    state(s),
    time(t)
{
    
}

CharacterEvent::CharacterEvent(const CharacterEvent& c) :
    character_index(c.character_index),
    state(c.state),
    time(c.time)
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

double CharacterEvent::getTime(void) const
{
    return time;
}

size_t CharacterEvent::getCharacterIndex(void) const
{
    return character_index;
}

size_t CharacterEvent::getState(void) const
{
    return state;
}

std::string CharacterEvent::getStateStr(void) const
{
    std::stringstream ss;
    // want to do this based on DiscreteDataType...
//    ss << (char)(70+state);
    ss << state;
    return ss.str();
}

void CharacterEvent::setCharacterIndex(size_t i)
{
    character_index = i;
}

void CharacterEvent::setState(size_t s)
{
    state = s;
}

void CharacterEvent::setTime(double t)
{
    time = t;
}

void CharacterEvent::print(void) const
{
    std::cout << character_index << " " << state << " " << time << "\n";
}
